#pragma once

#include "../action.h"
#include "../board_data.h"

namespace MineChess::Actions
{
	class RevokeCastlingRights : public Action
	{
	public:
		const Color color;
		const CastlingType castling_type;
		RevokeCastlingRights(const Color p_color, const CastlingType p_castling_type);

	private:
		bool _was_performed = false;

	public:
		void Perform(BoardData &p_board_data) final;
		void Undo(BoardData &p_board_data) const final;
		void Redo(BoardData &p_board_data) const final;
	};
} //namespace MineChess::Actions
