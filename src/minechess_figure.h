#pragma once

#include "godot_cpp/classes/ref_counted.hpp"
#include "godot_cpp/core/binder_common.hpp"
#include "minechess/board_data.h"
#include "minechess_position.h"

using namespace godot;

struct MineChessFigure : public RefCounted
{
	GDCLASS(MineChessFigure, RefCounted);

public:
	enum Color : bool
	{
		BLACK,
		WHITE
	};

	enum FigureType : unsigned char
	{
		PAWN = 0,
		ROOK,
		KNIGHT,
		BISHOP,
		QUEEN,
		KING
	};

private:
	MineChess::FigureType _figure_type;
	MineChess::Color _color;
	Ref<MineChessPosition> _position;
	unsigned char _mine_count;
	MineChessFigure::FigureType _get_figure_type() const;
	MineChessFigure::Color _get_color() const;
	unsigned char _get_mine_count() const;
	Ref<MineChessPosition> _get_position() const;

public:
	static Ref<MineChessFigure> create(MineChess::FigureType p_figure_type, MineChess::Color p_color, unsigned char p_mine_count, Ref<MineChessPosition> p_position);
	static Ref<MineChessFigure> create(const MineChess::PositionedFigure &p_figure, unsigned char p_mine_count);
	static Ref<MineChessFigure> create(const MineChess::FigureData &p_figure, const MineChess::Position &p_position, unsigned char p_mine_count);

protected:
	static void _bind_methods();
};

VARIANT_ENUM_CAST(MineChessFigure::Color)
VARIANT_ENUM_CAST(MineChessFigure::FigureType)
