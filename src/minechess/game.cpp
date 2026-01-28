#include "game.h"
#include "board_data.h"

#include "../macros/foreach.h"
#include "moves.h"
#include <vector>

using namespace MineChess;

Game::Game(const BoardSettings &p_settings, ExplosionResponder &p_explosion_responder, GameOverResponder &p_game_over_responder) :
		_board(p_settings, p_explosion_responder),
		_game_over_responder(p_game_over_responder)
{
}

Game::~Game()
{
	while (!_redo_stack.empty())
	{
		Move *move = _redo_stack.top();
		_redo_stack.pop();
		delete move;
	}

	while (!_undo_stack.empty())
	{
		Move *move = _undo_stack.top();
		_undo_stack.pop();
		delete move;
	}
}

bool Game::IsGameOver() const
{
	return _game_over_state.has_value();
}

std::optional<GameEndState> Game::GetGameEndState() const
{
	return _game_over_state;
}

Color Game::GetCurrentPlayer() const
{
	return _board.GetCurrentPlayer();
}

int Game::GetMineCount(const Position &p_position) const
{
	return _board.GetMineCount(p_position);
}

std::optional<FigureData> Game::GetFigure(const Position &p_position) const
{
	return _board.GetFigure(p_position);
}

void Game::GetFigures(std::vector<PositionedFigure> &figures) const
{
	_board.GetFigures(figures);
}

void Game::PerformMove(Move *move)
{
	move->Perform(_board);
	_undo_stack.push(move);

	while (!_redo_stack.empty())
	{
		Move *move = _redo_stack.top();
		_redo_stack.pop();
		delete move;
	}

	_board.ToggleCurrentPlayer();

	HandleGameOver();
}

void Game::HandleGameOver()
{
	bool white_has_king = Moves::HasKing(_board, WHITE);
	bool black_has_king = Moves::HasKing(_board, BLACK);

	if (!white_has_king && !black_has_king)
	{
		EndGame(EXPLOSION, DRAW);
		return;
	}
	if (!white_has_king)
	{
		EndGame(EXPLOSION, BLACK_WON);
		return;
	}
	if (!black_has_king)
	{
		EndGame(EXPLOSION, WHITE_WON);
		return;
	}

	if (!Moves::HasMaterial(_board, WHITE) && !Moves::HasMaterial(_board, BLACK))
	{
		EndGame(INSUFFICIENT_MATERIAL, DRAW);
		return;
	}

	if (GetCurrentPlayer() == WHITE)
	{
		if (Moves::IsInCheck(_board, BLACK))
		{
			EndGame(FAILED_DEFENSE, WHITE_WON);
			return;
		}
		if (!Moves::HasMoves(_board, WHITE))
		{
			if (Moves::IsInCheck(_board, WHITE))
			{
				EndGame(CHECKMATE, BLACK_WON);
				return;
			}
			else
			{
				EndGame(STALEMATE, DRAW);
				return;
			}
		}
	}

	if (GetCurrentPlayer() == BLACK)
	{
		if (Moves::IsInCheck(_board, WHITE))
		{
			EndGame(FAILED_DEFENSE, BLACK_WON);
			return;
		}
		if (!Moves::HasMoves(_board, BLACK))
		{
			if (Moves::IsInCheck(_board, BLACK))
			{
				EndGame(CHECKMATE, WHITE_WON);
				return;
			}
			else
			{
				EndGame(STALEMATE, DRAW);
				return;
			}
		}
	}
}

void Game::EndGame(EndReason p_reason, EndResult p_result)
{
	_game_over_state = GameEndState{ p_reason, p_result };
	_game_over_responder.OnGameOver(*_game_over_state);
}

// checks legality and performs a move if it's legal.
// returns true, if the move was performed successfully
// returns false, if the move failed. (The board state is not altered in this case)
// Promotion defaults to a queen
bool Game::TryMoveFigure(const Position &p_from, const Position &p_to)
{
	return TryMoveFigure(p_from, p_to, QUEEN);
}

// Checks legality and Performs a move using the provided promotion type.
bool Game::TryMoveFigure(const Position &p_from, const Position &p_to, FigureType p_to_promote_to)
{
	if (_game_over_state)
	{
		return false;
	}

	std::optional<FigureData> figure = _board.GetFigure(p_from);

	// Move is only legal if there's a figure at the starting position.
	if (!figure)
	{
		return false;
	}

	// Move is only legal if the start and end positions are different from one another.
	if (p_from.file == p_to.file && p_from.rank == p_to.rank)
	{
		return false;
	}

	// Check weak legality (Movement rules are followed)
	if (!Moves::IsWeakLegalMove(_board, *figure, p_from, p_to))
	{
		return false;
	}

	// Check strong legality (Player's king is not under attack afterwards)
	if (!Moves::IsStrongLegal(_board, *figure, p_from, p_to))
	{
		return false;
	}

	PerformMove(Moves::ConstructMove(_board, *figure, p_from, p_to, p_to_promote_to, Moves::MineHandling::BOARD_DEPENDEND));
	return true;
}

// Returns all positions a figure can be legally moved to.
// Returns an empty vector if the figure must not be moved.
void Game::GetLegalMoves(const Position &p_from, std::vector<Position> &strong_legal_moves)
{
	if (_game_over_state)
	{
		return;
	}

	std::optional<FigureData> figure_data = _board.GetFigure(p_from);
	if (!figure_data)
	{
		return;
	}

	// Only figures of the current player's color can be moved.
	if (figure_data->color != _board.GetCurrentPlayer())
	{
		return;
	}

	std::vector<Position> weak_legal_moves = std::vector<Position>();
	Moves::GetWeakLegalMoves(_board, *figure_data, p_from, weak_legal_moves);

	FOREACH(weak_legal_move, weak_legal_moves)
	{
		if (Moves::IsStrongLegal(_board, *figure_data, p_from, *weak_legal_move))
		{
			strong_legal_moves.push_back(*weak_legal_move);
		}
	}
}

bool Game::IsInCheck(Position position, Color color) const
{
	return Moves::IsInCheck(_board, position, color);
}

bool Game::CanUndo() const
{
	if (_game_over_state)
	{
		return false;
	}
	return !_undo_stack.empty();
}

void Game::Undo()
{
	if (_game_over_state)
	{
		return;
	}
	Move *move = _undo_stack.top();
	_undo_stack.pop();
	move->Undo(_board);
	_redo_stack.push(move);
	_board.ToggleCurrentPlayer();
}

bool Game::CanRedo() const
{
	if (_game_over_state)
	{
		return false;
	}
	return !_redo_stack.empty();
}

void Game::Redo()
{
	if (_game_over_state)
	{
		return;
	}
	Move *move = _redo_stack.top();
	_redo_stack.pop();
	move->Redo(_board);
	_undo_stack.push(move);
	_board.ToggleCurrentPlayer();
}

bool Game::HasLargeExplosions() const
{
	return _board.large_explosions;
}

bool Game::CanKingDefuse() const
{
	return _board.king_can_defuse;
}

bool Game::DoMinesRegenerate() const
{
	return _board.mine_regeneration;
}
