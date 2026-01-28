#include "remove_neighbours.h"
#include "../../macros/foreach.h"

using namespace MineChess::Actions;

RemoveNeighbours::RemoveNeighbours(const Position &p_position) :
		position(p_position)
{
}

void RemoveNeighbours::Perform(BoardData &p_board)
{
	FOREACH(direction, Position::neighbour_directions)
	{
		std::optional<Position> neighbour = position + *direction;
		if (!neighbour)
		{
			continue;
		}

		std::optional<FigureData> figure = p_board.GetFigure(*neighbour);
		if (!figure)
		{
			continue;
		}

		_removed_figures.push_back(PositionedFigure{ *neighbour, *figure });
		p_board.RemoveFigure(*neighbour);
	}
}

void RemoveNeighbours::Undo(BoardData &p_board) const
{
	FOREACH(removed_figure, _removed_figures)
	{
		p_board.PlaceFigure(removed_figure->position, removed_figure->figure);
	}
}

void RemoveNeighbours::Redo(BoardData &p_board) const
{
	FOREACH(removed_figure, _removed_figures)
	{
		p_board.RemoveFigure(removed_figure->position);
	}
}
