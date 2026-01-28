#include "place_figure.h"

using namespace MineChess::Actions;

PlaceFigure::PlaceFigure(const Position &p_position, const FigureData &p_figure_data) :
		position(p_position),
		figure_data(p_figure_data)
{
}

void PlaceFigure::Perform(BoardData &board)
{
	_was_performed = board.PlaceFigure(position, figure_data);
}

void PlaceFigure::Undo(BoardData &board) const
{
	if (_was_performed)
	{
		board.RemoveFigure(position);
	}
}

void PlaceFigure::Redo(BoardData &board) const
{
	if (_was_performed)
	{
		board.PlaceFigure(position, figure_data);
	}
}
