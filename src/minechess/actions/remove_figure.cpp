#include "remove_figure.h"

using namespace MineChess::Actions;

RemoveFigure::RemoveFigure(const Position &p_position) :
		position(p_position)
{
}

void RemoveFigure::Perform(BoardData &board)
{
	std::optional<FigureData> figure = board.GetFigure(position);

	if (figure)
	{
		_figure_data = figure;
		board.RemoveFigure(position);
		_was_performed = true;
	}
}

void RemoveFigure::Undo(BoardData &board) const
{
	if (_was_performed)
	{
		board.PlaceFigure(position, *_figure_data);
	}
}

void RemoveFigure::Redo(BoardData &board) const
{
	if (_was_performed)
	{
		board.RemoveFigure(position);
	}
}
