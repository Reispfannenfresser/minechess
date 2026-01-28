#include "grant_en_passant_rights.h"

using namespace MineChess::Actions;

GrantEnPassantRights::GrantEnPassantRights(File p_file) :
		file(p_file)
{
}

void GrantEnPassantRights::Perform(BoardData &p_board)
{
	_was_performed = p_board.GrantEnPassantRights(file);
}

void GrantEnPassantRights::Undo(BoardData &p_board) const
{
	if (_was_performed)
	{
		p_board.RevokeEnPassantRights();
	}
}

void GrantEnPassantRights::Redo(BoardData &p_board) const
{
	if (_was_performed)
	{
		p_board.GrantEnPassantRights(file);
	}
}
