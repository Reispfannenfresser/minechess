#include "revoke_castling_rights.h"

using namespace MineChess::Actions;

RevokeCastlingRights::RevokeCastlingRights(const Color p_color, const CastlingType p_castling_type) :
		color(p_color),
		castling_type(p_castling_type)
{
}

void RevokeCastlingRights::Perform(BoardData &p_board_data)
{
	_was_performed = p_board_data.RevokeCastlingRights(color, castling_type);
}

void RevokeCastlingRights::Undo(BoardData &p_board_data) const
{
	if (_was_performed)
	{
		p_board_data.GrantCastlingRights(color, castling_type);
	}
}

void RevokeCastlingRights::Redo(BoardData &p_board_data) const
{
	if (_was_performed)
	{
		p_board_data.RevokeCastlingRights(color, castling_type);
	}
}
