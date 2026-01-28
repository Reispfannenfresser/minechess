#include "cause_explosion.h"

using namespace MineChess::Actions;

CauseExplosion::CauseExplosion(const Position &p_position) :
		position(p_position)
{
}

void CauseExplosion::Perform(BoardData &p_board)
{
	p_board.CauseExplosion(position);
}

void CauseExplosion::Undo(BoardData &p_board) const
{
}

void CauseExplosion::Redo(BoardData &p_board) const
{
	p_board.CauseExplosion(position);
}
