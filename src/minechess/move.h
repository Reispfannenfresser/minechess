#pragma once

#include "action.h"
#include "board_data.h"
#include <vector>

namespace MineChess
{
	class Move
	{
	private:
		const std::vector<Action *> _actions;

	public:
		Move(const std::vector<Action *> &p_actions);
		~Move();
		void Perform(BoardData &p_board);
		void Undo(BoardData &p_board) const;
		void Redo(BoardData &p_board) const;
	};
} //namespace MineChess
