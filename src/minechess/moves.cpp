#include "moves.h"
#include "../macros/foreach.h"

#include "actions/cause_explosion.h"
#include "actions/grant_en_passant_rights.h"
#include "actions/place_figure.h"
#include "actions/place_random_mine.h"
#include "actions/promote.h"
#include "actions/remove_figure.h"
#include "actions/remove_mine.h"
#include "actions/remove_neighbours.h"
#include "actions/revoke_castling_rights.h"
#include "actions/revoke_en_passant_rights.h"

#include "board_data.h"
#include <cmath>
#include <cstdlib>
#include <vector>

using namespace MineChess;

void Moves::GetWeakLegalMoves(const BoardData &p_board, const FigureData &p_figure, const Position &p_from, std::vector<Position> &p_moves)
{
	switch (p_figure.figure_type)
	{
		case PAWN:
			Moves::GetWeakLegalPawnMoves(p_board, p_figure, p_from, p_moves);
			break;
		case ROOK:
			Moves::GetWeakLegalRookMoves(p_board, p_figure, p_from, p_moves);
			break;
		case KNIGHT:
			Moves::GetWeakLegalKnightMoves(p_board, p_figure, p_from, p_moves);
			break;
		case BISHOP:
			Moves::GetWeakLegalBishopMoves(p_board, p_figure, p_from, p_moves);
			break;
		case QUEEN:
			Moves::GetWeakLegalQueenMoves(p_board, p_figure, p_from, p_moves);
			break;
		case KING:
			Moves::GetWeakLegalKingMoves(p_board, p_figure, p_from, p_moves);
			break;
	}
}

void Moves::GetWeakLegalPawnMoves(const BoardData &p_board, const FigureData &p_figure, const Position &p_from, std::vector<Position> &moves)
{
	Rank en_passant_rank;
	Rank start_rank;
	Direction move_forward;
	const Direction move_right = Direction{ 1, 0 };
	const Direction move_left = Direction{ -1, 0 };
	const std::optional<File> en_passant_rights = p_board.GetEnpassantRights();

	switch (p_figure.color)
	{
		case MineChess::BLACK:
			en_passant_rank = MineChess::RANK_4;
			start_rank = MineChess::RANK_7;
			move_forward = Direction{ 0, -1 };
			break;
		case MineChess::WHITE:
			en_passant_rank = MineChess::RANK_5;
			start_rank = MineChess::RANK_2;
			move_forward = Direction{ 0, 1 };
			break;
	}

	std::optional<Position> forward = p_from + move_forward;
	if (!forward)
	{
		return;
	}

	if (!p_board.GetFigure(*forward))
	{
		moves.push_back(*forward);

		if (p_from.rank == start_rank)
		{
			std::optional<Position> forward_forward = *forward + move_forward;
			if (forward_forward && !p_board.GetFigure(*forward_forward))
			{
				moves.push_back(*forward_forward);
			}
		}
	}

	std::optional<Position> forward_right = *forward + move_right;
	if (forward_right)
	{
		std::optional<FigureData> forward_right_figure = p_board.GetFigure(*forward_right);
		if (forward_right_figure && forward_right_figure->color != p_figure.color)
		{
			moves.push_back(*forward_right);
		}
		else if (en_passant_rights && (*en_passant_rights) == forward_right->file && p_from.rank == en_passant_rank)
		{
			std::optional<Position> right = p_from + move_right;
			std::optional<FigureData> right_figure = p_board.GetFigure(*right);
			if (right_figure && right_figure->color != p_figure.color && right_figure->figure_type == MineChess::PAWN)
			{
				moves.push_back(*forward_right);
			}
		}
	}

	std::optional<Position> forward_left = *forward + move_left;
	if (forward_left)
	{
		std::optional<FigureData> forward_left_figure = p_board.GetFigure(*forward_left);
		if (forward_left_figure && forward_left_figure->color != p_figure.color)
		{
			moves.push_back(*forward_left);
		}
		else if (en_passant_rights && (*en_passant_rights) == forward_left->file && p_from.rank == en_passant_rank)
		{
			std::optional<Position> left = p_from + move_left;
			std::optional<FigureData> left_figure = p_board.GetFigure(*left);
			if (left_figure && left_figure->color != p_figure.color && left_figure->figure_type == MineChess::PAWN)
			{
				moves.push_back(*forward_left);
			}
		}
	}
}

void Moves::GetWeakLegalRookMoves(const BoardData &p_board, const FigureData &p_figure, const Position &p_from, std::vector<Position> &moves)
{
	FOREACH(direction, Position::rook_directions)
	{
		std::optional<Position> position = p_from + *direction;

		while (position)
		{
			std::optional<FigureData> figure = p_board.GetFigure(*position);

			if (figure && figure->color == p_figure.color)
			{
				break;
			}

			moves.push_back(*position);

			if (figure && figure->color != p_figure.color)
			{
				break;
			}

			position = *position + *direction;
		}
	}
}

void Moves::GetWeakLegalKnightMoves(const BoardData &p_board, const FigureData &p_figure, const Position &p_from, std::vector<Position> &moves)
{
	FOREACH(direction, Position::knight_directions)
	{
		std::optional<Position> neighbour = p_from + *direction;
		if (!neighbour)
		{
			// position invalid
			continue;
		}

		std::optional<FigureData> neighbour_figure = p_board.GetFigure(*neighbour);
		if (neighbour_figure && neighbour_figure->color == p_figure.color)
		{
			// position occupied by figure of the same color
			continue;
		}

		moves.push_back(*neighbour);
	}
}

void Moves::GetWeakLegalBishopMoves(const BoardData &p_board, const FigureData &p_figure, const Position &p_from, std::vector<Position> &moves)
{
	FOREACH(direction, Position::bishop_directions)
	{
		std::optional<Position> position = p_from + *direction;

		while (position)
		{
			std::optional<FigureData> figure = p_board.GetFigure(*position);

			if (figure && figure->color == p_figure.color)
			{
				break;
			}

			moves.push_back(*position);

			if (figure && figure->color != p_figure.color)
			{
				break;
			}

			position = *position + *direction;
		}
	}
}

void Moves::GetWeakLegalQueenMoves(const BoardData &p_board, const FigureData &p_figure, const Position &p_from, std::vector<Position> &moves)
{
	FOREACH(direction, Position::neighbour_directions)
	{
		std::optional<Position> position = p_from + *direction;

		while (position)
		{
			std::optional<FigureData> figure = p_board.GetFigure(*position);

			if (figure && figure->color == p_figure.color)
			{
				break;
			}

			moves.push_back(*position);

			if (figure && figure->color != p_figure.color)
			{
				break;
			}

			position = *position + *direction;
		}
	}
}

void Moves::GetWeakLegalKingMoves(const BoardData &p_board, const FigureData &p_figure, const Position &p_from, std::vector<Position> &moves)
{
	FOREACH(direction, Position::neighbour_directions)
	{
		std::optional<Position> neighbour = p_from + *direction;
		if (!neighbour)
		{
			// position invalid
			continue;
		}

		std::optional<FigureData> neighbour_figure = p_board.GetFigure(*neighbour);
		if (neighbour_figure && neighbour_figure->color == p_figure.color)
		{
			// position occupied by figure of the same color
			continue;
		}

		moves.push_back(*neighbour);
	}

	Position start;

	switch (p_figure.color)
	{
		case WHITE:
			start = Position{ FILE_E, RANK_1 };
			break;
		case BLACK:
			start = Position{ FILE_E, RANK_8 };
			break;
	}

	if (p_from == start)
	{
		if (p_board.CanCastle(p_figure.color, SHORT_CASTLE))
		{
			std::optional<FigureData> rook = p_board.GetFigure(Position{ FILE_H, p_from.rank });
			if (rook && rook->figure_type == MineChess::ROOK && rook->color == p_figure.color &&
					!p_board.GetFigure(Position{ FILE_F, p_from.rank }) &&
					!p_board.GetFigure(Position{ FILE_G, p_from.rank }))
			{
				moves.push_back(Position{ FILE_G, p_from.rank });
			}
		}
		if (p_board.CanCastle(p_figure.color, LONG_CASTLE))
		{
			std::optional<FigureData> rook = p_board.GetFigure(Position{ FILE_A, p_from.rank });
			if (rook && rook->figure_type == MineChess::ROOK && rook->color == p_figure.color &&
					!p_board.GetFigure(Position{ FILE_B, p_from.rank }) &&
					!p_board.GetFigure(Position{ FILE_C, p_from.rank }) &&
					!p_board.GetFigure(Position{ FILE_D, p_from.rank }))
			{
				moves.push_back(Position{ FILE_C, p_from.rank });
			}
		}
	}
}

bool Moves::IsWeakLegalMove(const BoardData &p_board, const FigureData &p_figure, const Position &p_from, const Position &p_to)
{
	switch (p_figure.figure_type)
	{
		case PAWN:
			return Moves::IsWeakLegalPawnMove(p_board, p_figure, p_from, p_to);

		case ROOK:
			return Moves::IsWeakLegalRookMove(p_board, p_figure, p_from, p_to);

		case KNIGHT:
			return Moves::IsWeakLegalKnightMove(p_board, p_figure, p_from, p_to);

		case BISHOP:
			return Moves::IsWeakLegalBishopMove(p_board, p_figure, p_from, p_to);

		case QUEEN:
			return Moves::IsWeakLegalQueenMove(p_board, p_figure, p_from, p_to);

		case KING:
			return Moves::IsWeakLegalKingMove(p_board, p_figure, p_from, p_to);

		default:
			return false;
	}
}

bool Moves::IsWeakLegalPawnMove(const BoardData &p_board, const FigureData &p_figure, const Position &p_from, const Position &p_to)
{
	std::vector<Position> moves = std::vector<Position>();
	Moves::GetWeakLegalPawnMoves(p_board, p_figure, p_from, moves);

	FOREACH(position, moves)
	{
		if (*position == p_to)
		{
			return true;
		}
	}

	return false;
}

bool Moves::IsWeakLegalRookMove(const BoardData &p_board, const FigureData &p_figure, const Position &p_from, const Position &p_to)
{
	if (p_from == p_to)
	{
		return false;
	}

	char file_offset = p_to.file - p_from.file;
	char rank_offset = p_to.rank - p_from.rank;

	Direction direction;
	if (file_offset == 0)
	{
		direction = Direction{ 0, static_cast<char>((rank_offset > 0) * 2 - 1) };
	}
	else if (rank_offset == 0)
	{
		direction = Direction{ static_cast<char>((file_offset > 0) * 2 - 1), 0 };
	}
	else
	{
		return false;
	}

	std::optional<Position> position = p_from + direction;
	while (position)
	{
		std::optional<FigureData> figure = p_board.GetFigure(*position);

		if ((*position) == p_to && (!figure || figure->color != p_figure.color))
		{
			return true;
		}
		if (figure)
		{
			return false;
		}

		position = *position + direction;
	}

	return true;
}

bool Moves::IsWeakLegalKnightMove(const BoardData &p_board, const FigureData &p_figure, const Position &p_from, const Position &p_to)
{
	if (p_from == p_to)
	{
		return false;
	}

	char abs_file_offset = std::abs(p_to.file - p_from.file);
	char abs_rank_offset = std::abs(p_to.rank - p_from.rank);

	if (!((abs_file_offset == 1 && abs_rank_offset == 2) || (abs_file_offset == 2 && abs_rank_offset == 1)))
	{
		return false;
	}

	std::optional<FigureData> to_figure = p_board.GetFigure(p_to);

	if (to_figure && to_figure->color == p_figure.color)
	{
		return false;
	}

	return true;
}

bool Moves::IsWeakLegalBishopMove(const BoardData &p_board, const FigureData &p_figure, const Position &p_from, const Position &p_to)
{
	if (p_from == p_to)
	{
		return false;
	}

	char file_offset = p_to.file - p_from.file;
	char rank_offset = p_to.rank - p_from.rank;

	if (std::abs(file_offset) != std::abs(rank_offset))
	{
		return false;
	}

	Direction direction = Direction{ static_cast<char>((file_offset > 0) * 2 - 1), static_cast<char>((rank_offset > 0) * 2 - 1) };

	std::optional<Position> position = p_from + direction;
	while (position)
	{
		std::optional<FigureData> figure = p_board.GetFigure(*position);

		if ((*position) == p_to && (!figure || figure->color != p_figure.color))
		{
			return true;
		}
		if (figure)
		{
			return false;
		}

		position = *position + direction;
	}

	return true;
}

bool Moves::IsWeakLegalQueenMove(const BoardData &p_board, const FigureData &p_figure, const Position &p_from, const Position &p_to)
{
	if (p_from == p_to)
	{
		return false;
	}

	char file_offset = p_to.file - p_from.file;
	char rank_offset = p_to.rank - p_from.rank;

	if (file_offset != 0 && rank_offset != 0 && std::abs(file_offset) != std::abs(rank_offset))
	{
		return false;
	}

	Direction direction;
	if (file_offset == 0)
	{
		direction = Direction{ 0, static_cast<char>((rank_offset > 0) * 2 - 1) };
	}
	else if (rank_offset == 0)
	{
		direction = Direction{ static_cast<char>((file_offset > 0) * 2 - 1), 0 };
	}
	else
	{
		direction = Direction{ static_cast<char>((file_offset > 0) * 2 - 1), static_cast<char>((rank_offset > 0) * 2 - 1) };
	}

	std::optional<Position> position = p_from + direction;
	while (position)
	{
		std::optional<FigureData> figure = p_board.GetFigure(*position);

		if ((*position) == p_to && (!figure || figure->color != p_figure.color))
		{
			return true;
		}
		else if (figure)
		{
			return false;
		}

		position = *position + direction;
	}
	return true;
}

bool Moves::IsWeakLegalKingMove(const BoardData &p_board, const FigureData &p_figure, const Position &p_from, const Position &p_to)
{
	if (p_from == p_to)
	{
		return false;
	}

	Position start;
	Direction offset = Direction{ 2, 0 };

	switch (p_figure.color)
	{
		case WHITE:
			start = Position{ FILE_E, RANK_1 };
			break;
		case BLACK:
			start = Position{ FILE_E, RANK_8 };
			break;
	}

	std::optional<Position> short_castle = start + offset;
	std::optional<Position> long_castle = start - offset;

	if (p_from == start)
	{
		if (p_board.CanCastle(p_figure.color, SHORT_CASTLE) && short_castle && p_to == *short_castle)
		{
			std::optional<FigureData> rook = p_board.GetFigure(Position{ FILE_H, start.rank });

			return rook &&
					rook->figure_type == ROOK &&
					rook->color == p_figure.color &&
					!p_board.GetFigure(Position{ FILE_F, start.rank }) &&
					!p_board.GetFigure(Position{ FILE_G, start.rank });
		}

		if (p_board.CanCastle(p_figure.color, LONG_CASTLE) && long_castle && p_to == *long_castle)
		{
			std::optional<FigureData> rook = p_board.GetFigure(Position{ FILE_A, start.rank });

			return rook &&
					rook->figure_type == ROOK &&
					rook->color == p_figure.color &&
					!p_board.GetFigure(Position{ FILE_B, start.rank }) &&
					!p_board.GetFigure(Position{ FILE_C, start.rank }) &&
					!p_board.GetFigure(Position{ FILE_D, start.rank });
		}
	}

	char file_offset = p_to.file - p_from.file;
	char rank_offset = p_to.rank - p_from.rank;

	if (std::abs(rank_offset) > 1 || std::abs(file_offset) > 1)
	{
		return false;
	}

	std::optional<FigureData> to_figure = p_board.GetFigure(p_to);
	if (to_figure && to_figure->color == p_figure.color)
	{
		return false;
	}

	return true;
}

void Moves::GetStrongLegalMoves(BoardData &p_board, const FigureData &p_figure, const Position &p_from, std::vector<Position> &moves)
{
	std::vector<Position> weak_legal_moves = std::vector<Position>();
	Moves::GetWeakLegalMoves(p_board, p_figure, p_from, weak_legal_moves);

	FOREACH(weak_legal_move, weak_legal_moves)
	{
		if (Moves::IsStrongLegal(p_board, p_figure, p_from, *weak_legal_move))
		{
			moves.push_back(*weak_legal_move);
		}
	}

	return;
}

bool Moves::IsStrongLegal(BoardData &p_board, const FigureData &p_figure, const Position &p_from, const Position &p_to)
{
	if (p_figure.figure_type == KING)
	{
		char file_offset = p_to.file - p_from.file;
		if (file_offset == 2)
		{
			std::optional<Position> right_pos = p_from + Direction{ 1, 0 };
			if (IsInCheck(p_board, p_from, p_figure.color) || !right_pos || IsInCheck(p_board, *right_pos, p_figure.color) || IsInCheck(p_board, p_to, p_figure.color))
			{
				return false;
			}
		}
		else if (file_offset == -2)
		{
			std::optional<Position> left_pos = p_from + Direction{ -1, 0 };
			if (IsInCheck(p_board, p_from, p_figure.color) || !left_pos || IsInCheck(p_board, *left_pos, p_figure.color) || IsInCheck(p_board, p_to, p_figure.color))
			{
				return false;
			}
		}
	}

	Move *explosive_move = Moves::ConstructMove(p_board, p_figure, p_from, p_to, QUEEN, Moves::ALWAYS_EXPLODE);

	explosive_move->Perform(p_board);
	bool is_strong_legal = !Moves::IsInCheck(p_board, p_figure.color);
	explosive_move->Undo(p_board);
	delete explosive_move;

	if (is_strong_legal)
	{
		return true;
	}

	Move *inexplosive_move = Moves::ConstructMove(p_board, p_figure, p_from, p_to, QUEEN, Moves::NEVER_EXPLODE);
	inexplosive_move->Perform(p_board);
	is_strong_legal = !Moves::IsInCheck(p_board, p_figure.color);
	inexplosive_move->Undo(p_board);
	delete inexplosive_move;

	return is_strong_legal;
}

bool Moves::HasMaterial(BoardData &p_board, const Color p_color)
{
	bool mines_remaining = p_board.GetRemainingMineCount() > 0;

	if (!p_board.king_can_defuse && mines_remaining)
	{
		// King can still explode
		return true;
	}

	std::vector<PositionedFigure> figures = std::vector<PositionedFigure>();
	p_board.GetFigures(figures);

	if (p_board.large_explosions && mines_remaining)
	{
		FOREACH(figure, figures)
		{
			if (figure->figure.figure_type != KING)
			{
				// King can explode collaterally from a large explosion caused by any other piece
				return true;
			}
		}
	}
	else
	{
		bool has_one = false;

		FOREACH(figure, figures)
		{
			if (figure->figure.color == p_color)
			{
				switch (figure->figure.figure_type)
				{
					case KING:
						break;
					case PAWN:
					case ROOK:
					case QUEEN:
						// Rook, Queen or Pawn is enough to checkmate
						return true;
					case BISHOP:
					case KNIGHT:
						if (has_one)
						{
							// At least two pieces that are knights or bishops are required for checkmate
							return true;
						}
						has_one = true;
				}
			}
		}
	}
	return false;
}

bool Moves::HasMoves(BoardData &p_board, const Color p_color)
{
	std::vector<PositionedFigure> figures = std::vector<PositionedFigure>();
	p_board.GetFigures(figures);

	FOREACH(figure, figures)
	{
		if (figure->figure.color == p_color)
		{
			std::vector<Position> weak_legal_moves = std::vector<Position>();
			Moves::GetWeakLegalMoves(p_board, figure->figure, figure->position, weak_legal_moves);

			FOREACH(weak_legal_move, weak_legal_moves)
			{
				if (Moves::IsStrongLegal(p_board, figure->figure, figure->position, *weak_legal_move))
				{
					return true;
				}
			}
		}
	}

	return false;
}

bool Moves::HasKing(const BoardData &p_board, const Color p_color)
{
	std::vector<PositionedFigure> figures = std::vector<PositionedFigure>();
	p_board.GetFigures(figures);

	FOREACH(figure, figures)
	{
		if (figure->figure.figure_type == KING && figure->figure.color == p_color)
		{
			return true;
		}
	}
	return false;
}

bool Moves::IsInCheck(const BoardData &p_board, const Position &p_position, const Color p_color)
{
	std::vector<PositionedFigure> figures = std::vector<PositionedFigure>();
	p_board.GetFigures(figures);

	FOREACH(figure, figures)
	{
		if (figure->figure.color != p_color)
		{
			if (IsWeakLegalMove(p_board, figure->figure, figure->position, p_position))
			{
				return true;
			}
		}
	}
	return false;
}

bool Moves::IsInCheck(const BoardData &p_board, const Color color)
{
	std::vector<Position> king_positions = std::vector<Position>();
	std::vector<PositionedFigure> opponent_figures = std::vector<PositionedFigure>();

	std::vector<PositionedFigure> figures = std::vector<PositionedFigure>();
	p_board.GetFigures(figures);

	FOREACH(figure, figures)
	{
		if (figure->figure.color != color)
		{
			opponent_figures.push_back(*figure);
		}
		else if (figure->figure.figure_type == KING)
		{
			king_positions.push_back(figure->position);
			continue;
		}
	}

	if (king_positions.size() == 0)
	{
		return true;
	}

	FOREACH(king_position, king_positions)
	{
		FOREACH(opponent_figure, opponent_figures)
		{
			if (IsWeakLegalMove(p_board, opponent_figure->figure, opponent_figure->position, *king_position))
			{
				return true;
			}
		}
	}

	return false;
}

void Moves::MineHandling(const BoardData &p_board, const FigureData &p_figure, const Position &p_position, const enum Moves::MineHandling p_mine_handling, std::vector<Action *> &p_actions)
{
	bool is_board_dependent = p_mine_handling == BOARD_DEPENDEND;

	if (p_mine_handling == NEVER_EXPLODE)
	{
		return;
	}
	if (is_board_dependent && !p_board.IsMine(p_position))
	{
		return;
	}

	FOREACH(direction, Position::neighbour_directions)
	{
		std::optional<Position> neighbour = p_position + *direction;
		if (!neighbour)
		{
			continue;
		}
		if (p_board.GetFigure(*neighbour) && p_board.GetMineCount(*neighbour) == 0)
		{
			return;
		}
	}

	if (is_board_dependent && p_board.mine_regeneration)
	{
		p_actions.emplace_back(new Actions::PlaceRandomMine());
	}

	p_actions.emplace_back(new Actions::RemoveMine(p_position));

	if (p_figure.figure_type != KING || !p_board.king_can_defuse)
	{
		if (is_board_dependent)
		{
			p_actions.emplace_back(new Actions::CauseExplosion(p_position));
		}
		p_actions.emplace_back(new Actions::RemoveFigure(p_position));
		if (p_board.large_explosions)
		{
			p_actions.emplace_back(new Actions::RemoveNeighbours(p_position));
		}
	}
}

// Constructs a weak legal move with board dependent explosions and only queen promotions
Move *Moves::ConstructMove(const BoardData &p_board, const FigureData &p_figure, const Position &p_from, const Position &p_to)
{
	return ConstructMove(p_board, p_figure, p_from, p_to, QUEEN, BOARD_DEPENDEND);
}

// Constructs a weak legal move with specified promotion type and board dependent explosions
Move *Moves::ConstructMove(const BoardData &p_board, const FigureData &p_figure, const Position &p_from, const Position &p_to, FigureType to_promote_to)
{
	return ConstructMove(p_board, p_figure, p_from, p_to, to_promote_to, BOARD_DEPENDEND);
}

// Constructs a weak legal move with specified promotion type and specified mine handling
Move *Moves::ConstructMove(const BoardData &p_board, const FigureData &p_figure, const Position &p_from, const Position &p_to, FigureType to_promote_to, const enum Moves::MineHandling p_mine_handling)
{
	std::vector<Action *> actions = std::vector<Action *>();

	std::optional<FigureData> to_figure = p_board.GetFigure(p_to);

	actions.emplace_back(new Actions::RevokeEnPassantRights());
	actions.emplace_back(new Actions::RemoveFigure(p_from));

	if (to_figure)
	{
		actions.emplace_back(new Actions::RemoveFigure(p_to));
	}

	actions.emplace_back(new Actions::PlaceFigure(p_to, p_figure));

	// special pawn moves
	if (p_figure.figure_type == PAWN)
	{
		Rank promotion_rank = RANK_8;
		if (p_figure.color == BLACK)
		{
			promotion_rank = RANK_1;
		}

		// promotion
		if (p_to.rank == promotion_rank)
		{
			actions.emplace_back(new Actions::Promote(p_to, to_promote_to));
		}

		// en passant
		if (p_to.file != p_from.file && !to_figure)
		{
			actions.emplace_back(new Actions::RemoveFigure(Position{ p_to.file, p_from.rank }));
		}

		// start leap
		char abs_rank_offset = std::abs(p_to.rank - p_from.rank);
		if (abs_rank_offset > 1)
		{
			actions.emplace_back(new Actions::GrantEnPassantRights(p_to.file));
		}
	}

	// castling
	if (p_figure.figure_type == KING)
	{
		char abs_file_offset = std::abs(p_to.file - p_from.file);
		if (abs_file_offset > 1)
		{
			Position old_rook_pos;
			Position new_rook_pos;
			FigureData rook = FigureData{ ROOK, p_figure.color };

			if (p_to.file == FILE_G)
			{
				old_rook_pos = Position{ FILE_H, p_to.rank };
				new_rook_pos = Position{ FILE_F, p_to.rank };
			}
			if (p_to.file == FILE_C)
			{
				old_rook_pos = Position{ FILE_A, p_to.rank };
				new_rook_pos = Position{ FILE_D, p_to.rank };
			}
			actions.emplace_back(new Actions::RemoveFigure(old_rook_pos));
			actions.emplace_back(new Actions::PlaceFigure(new_rook_pos, rook));

			MineHandling(p_board, rook, new_rook_pos, p_mine_handling, actions);
		}
		actions.emplace_back(new Actions::RevokeCastlingRights(p_figure.color, LONG_CASTLE));
		actions.emplace_back(new Actions::RevokeCastlingRights(p_figure.color, SHORT_CASTLE));
	}

	if (p_figure.figure_type == ROOK)
	{
		Rank start_rank = RANK_1;
		if (p_figure.color == BLACK)
		{
			start_rank = RANK_8;
		}

		if (p_from.file == FILE_A && p_from.rank == start_rank)
		{
			actions.emplace_back(new Actions::RevokeCastlingRights(p_figure.color, LONG_CASTLE));
		}
		if (p_from.file == FILE_H && p_from.rank == start_rank)
		{
			actions.emplace_back(new Actions::RevokeCastlingRights(p_figure.color, SHORT_CASTLE));
		}
	}

	if (!to_figure)
	{
		MineHandling(p_board, p_figure, p_to, p_mine_handling, actions);
	}

	return new Move(actions);
}
