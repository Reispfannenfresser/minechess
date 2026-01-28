#include "minechess_figure.h"
#include "godot_cpp/core/class_db.hpp"
#include "godot_cpp/core/object.hpp"
#include "minechess/board_data.h"

using namespace godot;

MineChessFigure::FigureType MineChessFigure::_get_figure_type() const
{
	return (FigureType)_figure_type;
}

MineChessFigure::Color MineChessFigure::_get_color() const
{
	return (Color)_color;
}

unsigned char MineChessFigure::_get_mine_count() const
{
	return _mine_count;
}

Ref<MineChessPosition> MineChessFigure::_get_position() const
{
	return _position;
}

Ref<MineChessFigure> MineChessFigure::create(MineChess::FigureType p_figure_type, MineChess::Color p_color, unsigned char p_mine_count, Ref<MineChessPosition> p_position)
{
	Ref<MineChessFigure> figure;
	figure.instantiate();
	figure->_figure_type = p_figure_type;
	figure->_color = p_color;
	figure->_mine_count = p_mine_count;
	figure->_position = p_position;

	return figure;
}

Ref<MineChessFigure> MineChessFigure::create(const MineChess::PositionedFigure &p_figure, unsigned char p_mine_count)
{
	Ref<MineChessFigure> figure;
	figure.instantiate();
	figure->_figure_type = p_figure.figure.figure_type;
	figure->_color = p_figure.figure.color;
	figure->_mine_count = p_mine_count;
	figure->_position = MineChessPosition::create(p_figure.position);
	return figure;
}

Ref<MineChessFigure> MineChessFigure::create(const MineChess::FigureData &p_figure, const MineChess::Position &p_position, unsigned char p_mine_count)
{
	Ref<MineChessFigure> figure;
	figure.instantiate();
	figure->_figure_type = p_figure.figure_type;
	figure->_color = p_figure.color;
	figure->_mine_count = p_mine_count;
	figure->_get_position() = MineChessPosition::create(p_position);
	return figure;
}

void MineChessFigure::_bind_methods()
{
	ClassDB::bind_method(D_METHOD("get_figure_type"), &MineChessFigure::_get_figure_type);
	ClassDB::bind_method(D_METHOD("get_color"), &MineChessFigure::_get_color);
	ClassDB::bind_method(D_METHOD("get_mine_count"), &MineChessFigure::_get_mine_count);
	ClassDB::bind_method(D_METHOD("get_position"), &MineChessFigure::_get_position);

	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "color", PROPERTY_HINT_ENUM, "black,white"), "", "get_color");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "figure_type", PROPERTY_HINT_ENUM, "pawn,rook,knight,bishop,queen,king"), "", "get_figure_type");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "mine_count"), "", "get_mine_count");
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "position"), "", "get_position");

	BIND_ENUM_CONSTANT(PAWN);
	BIND_ENUM_CONSTANT(ROOK);
	BIND_ENUM_CONSTANT(KNIGHT);
	BIND_ENUM_CONSTANT(BISHOP);
	BIND_ENUM_CONSTANT(QUEEN);
	BIND_ENUM_CONSTANT(KING);

	BIND_ENUM_CONSTANT(BLACK);
	BIND_ENUM_CONSTANT(WHITE);
}
