#include "remove_mine.h"

using namespace MineChess::Actions;

RemoveMine::RemoveMine(const Position &p_position) :
		position(p_position)
{
}

void RemoveMine::Perform(BoardData &p_board)
{
	_was_performed = p_board.RemoveMine(position);
}

void RemoveMine::Undo(BoardData &p_board) const
{
	if (_was_performed)
	{
		p_board.PlaceMine(position);
	}
}

void RemoveMine::Redo(BoardData &p_board) const
{
	if (_was_performed)
	{
		p_board.RemoveMine(position);
	}
}
