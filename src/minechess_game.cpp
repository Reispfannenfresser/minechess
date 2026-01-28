#include "minechess_game.h"
#include "macros/foreach.h"
#include "minechess/board_data.h"
#include "minechess_figure.h"
#include "minechess_position.h"
#include <vector>

using namespace godot;

Ref<MineChessGame> MineChessGame::create(const int &p_mine_count, const bool &p_king_can_defuse, const bool &p_large_explosions, const bool &p_mine_regeneration, const int &p_seed)
{
	MineChess::BoardSettings board_settings = MineChess::BoardSettings{ p_mine_count, p_seed, p_king_can_defuse, p_large_explosions, p_mine_regeneration };
	Ref<MineChessGame> game;
	game.instantiate();
	game->_game = new MineChess::Game(board_settings, **game, **game);
	return game;
}

MineChessGame::~MineChessGame()
{
	delete _game;
}

bool MineChessGame::can_undo() const
{
	return _game->CanUndo();
}

bool MineChessGame::can_redo() const
{
	return _game->CanRedo();
}

void MineChessGame::undo()
{
	_game->Undo();
}

void MineChessGame::redo()
{
	_game->Redo();
}

void MineChessGame::OnExplosion(const MineChess::Position &p_position)
{
	emit_signal("explosion", MineChessPosition::create(p_position));
}

void MineChessGame::OnGameOver(const MineChess::GameEndState &p_game_end_state)
{
	emit_signal("game_ended", p_game_end_state.result, p_game_end_state.reason);
}

Ref<MineChessFigure> MineChessGame::get_figuredata(Ref<MineChessPosition> p_position) const
{
	MineChess::Position position = p_position->get_position();

	std::optional<MineChess::FigureData> figureData = _game->GetFigure(position);
	if (!figureData)
	{
		return nullptr;
	}
	unsigned int mine_count = _game->GetMineCount(position);
	return MineChessFigure::create(figureData->figure_type, figureData->color, mine_count, MineChessPosition::create(position));
}

bool MineChessGame::try_move_figure(Ref<MineChessPosition> from, Ref<MineChessPosition> to, MineChessFigure::FigureType figure_type)
{
	return _game->TryMoveFigure(from->get_position(), to->get_position(), MineChess::FigureType(figure_type));
}

TypedArray<Ref<MineChessPosition>> MineChessGame::get_legal_moves(Ref<MineChessPosition> from) const
{
	std::vector<MineChess::Position> legal_moves = std::vector<MineChess::Position>();
	_game->GetLegalMoves(from->get_position(), legal_moves);

	TypedArray<Ref<MineChessPosition>> result = TypedArray<Ref<MineChessPosition>>();

	FOREACH(legal_move, legal_moves)
	{
		result.push_back(MineChessPosition::create(*legal_move));
	}

	return result;
}

TypedArray<Ref<MineChessFigure>> MineChessGame::get_figures() const
{
	std::vector<MineChess::PositionedFigure> figures = std::vector<MineChess::PositionedFigure>();
	_game->GetFigures(figures);

	TypedArray<Ref<MineChessFigure>> result = TypedArray<Ref<MineChessFigure>>();

	FOREACH(figure, figures)
	{
		result.push_back(MineChessFigure::create(*figure, _game->GetMineCount(figure->position)));
	}
	return result;
}

MineChessFigure::Color MineChessGame::get_current_player() const
{
	if (_game->GetCurrentPlayer() == MineChess::WHITE)
	{
		return MineChessFigure::WHITE;
	}
	else
	{
		return MineChessFigure::BLACK;
	}
}

bool MineChessGame::is_in_check(Ref<MineChessPosition> position, MineChessFigure::Color color) const
{
	if (color == MineChessFigure::WHITE)
	{
		return _game->IsInCheck(position->get_position(), MineChess::WHITE);
	}
	else
	{
		return _game->IsInCheck(position->get_position(), MineChess::BLACK);
	}
}

bool MineChessGame::get_king_can_defuse() const
{
	return _game->CanKingDefuse();
}

bool MineChessGame::get_large_explosions() const
{
	return _game->HasLargeExplosions();
}

bool MineChessGame::get_mine_regeneration() const
{
	return _game->DoMinesRegenerate();
}

void MineChessGame::_bind_methods()
{
	ClassDB::bind_static_method("MineChessGame", D_METHOD("create", "mine_count", "king_can_defuse", "large_explosions", "mine_regeneration", "seed"), &MineChessGame::create);
	ClassDB::bind_method(D_METHOD("get_figuredata", "position"), &MineChessGame::get_figuredata);
	ClassDB::bind_method(D_METHOD("get_figures"), &MineChessGame::get_figures);
	ClassDB::bind_method(D_METHOD("get_current_player"), &MineChessGame::get_current_player);
	ClassDB::bind_method(D_METHOD("can_undo"), &MineChessGame::can_undo);
	ClassDB::bind_method(D_METHOD("can_redo"), &MineChessGame::can_redo);
	ClassDB::bind_method(D_METHOD("undo"), &MineChessGame::undo);
	ClassDB::bind_method(D_METHOD("redo"), &MineChessGame::redo);
	ClassDB::bind_method(D_METHOD("try_move_figure", "from", "to", "to_promote_to"), &MineChessGame::try_move_figure);
	ClassDB::bind_method(D_METHOD("get_legal_moves", "from"), &MineChessGame::get_legal_moves);
	ClassDB::bind_method(D_METHOD("is_in_check", "position", "color"), &MineChessGame::is_in_check);
	ClassDB::bind_method(D_METHOD("get_king_can_defuse"), &MineChessGame::get_king_can_defuse);
	ClassDB::bind_method(D_METHOD("get_large_explosions"), &MineChessGame::get_large_explosions);
	ClassDB::bind_method(D_METHOD("get_mine_regeneration"), &MineChessGame::get_mine_regeneration);

	ADD_SIGNAL(MethodInfo("explosion", PropertyInfo(Variant::OBJECT, "position")));
	ADD_SIGNAL(MethodInfo("game_ended", PropertyInfo(Variant::INT, "result"), PropertyInfo(Variant::INT, "reason")));

	BIND_ENUM_CONSTANT(CHECKMATE);
	BIND_ENUM_CONSTANT(STALEMATE);
	BIND_ENUM_CONSTANT(INSUFFICIENT_MATERIAL);
	BIND_ENUM_CONSTANT(EXPLOSION);
	BIND_ENUM_CONSTANT(FAILED_DEFENSE);

	BIND_ENUM_CONSTANT(WHITE_WON);
	BIND_ENUM_CONSTANT(BLACK_WON);
	BIND_ENUM_CONSTANT(DRAW);
}
