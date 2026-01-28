#include "revoke_en_passant_rights.h"

using namespace MineChess::Actions;

RevokeEnPassantRights::RevokeEnPassantRights()
{
}

void RevokeEnPassantRights::Perform(BoardData &p_board)
{
	std::optional<File> rights = p_board.GetEnpassantRights();
	if (rights)
	{
		p_board.RevokeEnPassantRights();
		_file = *rights;
		_was_performed = true;
	}
}

void RevokeEnPassantRights::Undo(BoardData &p_board) const
{
	if (_was_performed)
	{
		p_board.GrantEnPassantRights(_file);
	}
}

void RevokeEnPassantRights::Redo(BoardData &p_board) const
{
	if (_was_performed)
	{
		p_board.RevokeEnPassantRights();
	}
}
