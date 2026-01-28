#pragma once

#include "../action.h"
#include "../board_data.h"

namespace MineChess::Actions
{
	class CauseExplosion : public Action
	{
	public:
		const Position position;
		CauseExplosion(const Position &p_position);

	public:
		void Perform(BoardData &board) final;
		void Undo(BoardData &board) const final;
		void Redo(BoardData &board) const final;
	};
} //namespace MineChess::Actions
