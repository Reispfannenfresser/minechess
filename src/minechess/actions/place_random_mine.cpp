#include "place_random_mine.h"

using namespace MineChess::Actions;

PlaceRandomMine::PlaceRandomMine()
{
}

void PlaceRandomMine::Perform(BoardData &p_board)
{
	_position = p_board.GetRandomEmptySquare();
	if (_position)
	{
		p_board.PlaceMine(*_position);
	}
}

void PlaceRandomMine::Undo(BoardData &p_board) const
{
	if (_position)
	{
		p_board.RemoveMine(*_position);
	}
}

void PlaceRandomMine::Redo(BoardData &p_board) const
{
	if (_position)
	{
		p_board.PlaceMine(*_position);
	}
}
