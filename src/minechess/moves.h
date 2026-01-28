#pragma once

#include "board_data.h"
#include "move.h"

namespace MineChess::Moves
{

	enum MineHandling : unsigned char
	{
		BOARD_DEPENDEND = 0,
		ALWAYS_EXPLODE,
		NEVER_EXPLODE
	};

	void GetWeakLegalMoves(const BoardData &p_board, const FigureData &p_figure, const Position &p_from, std::vector<Position> &p_moves);
	void GetWeakLegalPawnMoves(const BoardData &p_board, const FigureData &p_figure, const Position &p_from, std::vector<Position> &p_moves);
	void GetWeakLegalRookMoves(const BoardData &p_board, const FigureData &p_figure, const Position &p_from, std::vector<Position> &p_moves);
	void GetWeakLegalKnightMoves(const BoardData &p_board, const FigureData &p_figure, const Position &p_from, std::vector<Position> &p_moves);
	void GetWeakLegalBishopMoves(const BoardData &p_board, const FigureData &p_figure, const Position &p_from, std::vector<Position> &p_moves);
	void GetWeakLegalQueenMoves(const BoardData &p_board, const FigureData &p_figure, const Position &p_from, std::vector<Position> &p_moves);
	void GetWeakLegalKingMoves(const BoardData &p_board, const FigureData &p_figure, const Position &p_from, std::vector<Position> &p_moves);

	bool IsWeakLegalMove(const BoardData &p_board, const FigureData &p_figure, const Position &p_from, const Position &p_to);
	bool IsWeakLegalPawnMove(const BoardData &p_board, const FigureData &p_figure, const Position &p_from, const Position &p_to);
	bool IsWeakLegalRookMove(const BoardData &p_board, const FigureData &p_figure, const Position &p_from, const Position &p_to);
	bool IsWeakLegalKnightMove(const BoardData &p_board, const FigureData &p_figure, const Position &p_from, const Position &p_to);
	bool IsWeakLegalBishopMove(const BoardData &p_board, const FigureData &p_figure, const Position &p_from, const Position &p_to);
	bool IsWeakLegalQueenMove(const BoardData &p_board, const FigureData &p_figure, const Position &p_from, const Position &p_to);
	bool IsWeakLegalKingMove(const BoardData &p_board, const FigureData &p_figure, const Position &p_from, const Position &p_to);

	void GetStrongLegalMoves(BoardData &p_board, const FigureData &p_figure, const Position &p_from, std::vector<Position> &moves);

	bool IsStrongLegal(BoardData &p_board, const FigureData &p_figure, const Position &p_from, const Position &p_to);

	bool HasMaterial(BoardData &p_board, const Color p_color);
	bool HasMoves(BoardData &p_board, const Color p_color);
	bool HasKing(const BoardData &p_board, const Color p_color);
	bool IsInCheck(const BoardData &p_board, const Position &p_position, const Color p_color);
	bool IsInCheck(const BoardData &p_board, const Color p_color);

	void MineHandling(const BoardData &p_board, const FigureData &p_figure, const Position &p_position, const MineHandling p_mine_handling, std::vector<Action *> &actions);

	Move *ConstructMove(const BoardData &p_board, const FigureData &p_figure, const Position &p_from, const Position &p_to);
	Move *ConstructMove(const BoardData &p_board, const FigureData &p_figure, const Position &p_from, const Position &p_to, FigureType to_promote_to);
	Move *ConstructMove(const BoardData &p_board, const FigureData &p_figure, const Position &p_from, const Position &p_to, FigureType to_promote_to, const enum MineHandling p_mine_handling);
} //namespace MineChess::Moves
