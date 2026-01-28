#include "promote.h"

using namespace MineChess::Actions;

Promote::Promote(const Position &p_position, FigureType p_promote_to) :
		position(p_position),
		promote_to(p_promote_to)
{
}

void Promote::Perform(BoardData &board)
{
	std::optional<FigureData> figure = board.GetFigure(position);

	if (figure && figure->figure_type == PAWN && figure->color == board.GetCurrentPlayer())
	{
		board.RemoveFigure(position);
		board.PlaceFigure(position, FigureData{ promote_to, figure->color });
		_was_performed = true;
	}
}

void Promote::Undo(BoardData &board) const
{
	if (_was_performed)
	{
		board.RemoveFigure(position);
		board.PlaceFigure(position, FigureData{ PAWN, board.GetCurrentPlayer() });
	}
}

void Promote::Redo(BoardData &board) const
{
	if (_was_performed)
	{
		board.RemoveFigure(position);
		board.PlaceFigure(position, FigureData{ promote_to, board.GetCurrentPlayer() });
	}
}
