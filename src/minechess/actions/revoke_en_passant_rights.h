#pragma once

#include "../action.h"
#include "../board_data.h"

namespace MineChess::Actions
{
	class RevokeEnPassantRights : public Action
	{
	public:
		RevokeEnPassantRights();

	private:
		File _file;
		bool _was_performed = false;

	public:
		void Perform(BoardData &board) final;
		void Undo(BoardData &board) const final;
		void Redo(BoardData &board) const final;
	};
} //namespace MineChess::Actions
