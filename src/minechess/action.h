#pragma once

#include "board_data.h"

namespace MineChess
{
	class Action
	{
	public:
		virtual ~Action();
		virtual void Perform(BoardData &board) = 0;
		virtual void Undo(BoardData &board) const = 0;
		virtual void Redo(BoardData &board) const = 0;
	};
} //namespace MineChess
