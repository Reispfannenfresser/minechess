#include "move.h"
#include "../macros/foreach.h"
#include "board_data.h"

using namespace MineChess;

Move::Move(const std::vector<Action *> &p_actions) :
		_actions(p_actions)
{
}

Move::~Move()
{
	FOREACH(action, _actions)
	{
		delete *action;
	}
}

void Move::Perform(BoardData &p_board)
{
	FOREACH(action, _actions)
	{
		(*action)->Perform(p_board);
	}
}

void Move::Undo(BoardData &p_board) const
{
	REVERSED_FOREACH(action, _actions)
	{
		(*action)->Undo(p_board);
	}
}

void Move::Redo(BoardData &p_board) const
{
	FOREACH(action, _actions)
	{
		(*action)->Redo(p_board);
	}
}
