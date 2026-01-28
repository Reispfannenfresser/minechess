#pragma once

#include "../action.h"

namespace MineChess::Actions
{
	class RemoveMine : public Action
	{
	public:
		const Position position;
		RemoveMine(const Position &p_position);

	private:
		bool _was_performed = false;

	public:
		void Perform(BoardData &p_board) final;
		void Undo(BoardData &p_board) const final;
		void Redo(BoardData &p_board) const final;
	};
} //namespace MineChess::Actions
