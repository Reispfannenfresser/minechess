#pragma once

#include "../action.h"

namespace MineChess::Actions
{
	class PlaceFigure : public Action
	{
	public:
		const Position position;
		const FigureData figure_data;
		PlaceFigure(const Position &p_position, const FigureData &p_figure_data);

	private:
		bool _was_performed = false;

	public:
		void Perform(BoardData &board) final;
		void Undo(BoardData &board) const final;
		void Redo(BoardData &board) const final;
	};
} //namespace MineChess::Actions
