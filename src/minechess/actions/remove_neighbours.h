#pragma once

#include "../action.h"
#include <vector>

namespace MineChess::Actions
{
	class RemoveNeighbours : public Action
	{
	public:
		const Position position;
		RemoveNeighbours(const Position &p_position);

	private:
		std::vector<PositionedFigure> _removed_figures;

	public:
		void Perform(BoardData &p_board) final;
		void Undo(BoardData &p_board) const final;
		void Redo(BoardData &p_board) const final;
	};
} //namespace MineChess::Actions
