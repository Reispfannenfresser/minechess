#pragma once

#include "../action.h"

namespace MineChess::Actions
{
	class Promote : public Action
	{
	public:
		const Position position;
		const FigureType promote_to;
		Promote(const Position &p_position, FigureType p_promote_to);

	private:
		bool _was_performed = false;

	public:
		void Perform(BoardData &board) final;
		void Undo(BoardData &board) const final;
		void Redo(BoardData &board) const final;
	};
} //namespace MineChess::Actions
