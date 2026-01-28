#pragma once

#include "../action.h"

namespace MineChess::Actions
{
	class PlaceRandomMine : public Action
	{
	public:
		PlaceRandomMine();

	private:
		std::optional<Position> _position;

	public:
		void Perform(BoardData &p_board) final;
		void Undo(BoardData &p_board) const final;
		void Redo(BoardData &p_board) const final;
	};
} //namespace MineChess::Actions
