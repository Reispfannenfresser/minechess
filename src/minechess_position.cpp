#include "minechess_position.h"
#include "godot_cpp/core/class_db.hpp"
#include "godot_cpp/core/object.hpp"
#include "minechess/board_data.h"

using namespace godot;

Ref<MineChessPosition> MineChessPosition::_create(File p_file, Rank p_rank)
{
	Ref<MineChessPosition> position;
	position.instantiate();
	position->_position = { MineChess::File(p_file), MineChess::Rank(p_rank) };

	return position;
}

MineChessPosition::File MineChessPosition::_get_file() const
{
	return (File)_position.file;
}

MineChessPosition::Rank MineChessPosition::_get_rank() const
{
	return (Rank)_position.rank;
}

unsigned char MineChessPosition::_get_index() const
{
	return _position.GetIndex();
}

Ref<MineChessPosition> MineChessPosition::create(const MineChess::Position &p_position)
{
	Ref<MineChessPosition> position;
	position.instantiate();
	position->_position = p_position;

	return position;
}

Ref<MineChessPosition> MineChessPosition::create(MineChess::File p_file, MineChess::Rank p_rank)
{
	Ref<MineChessPosition> position;
	position.instantiate();
	position->_position = { p_file, p_rank };

	return position;
}

MineChess::Position MineChessPosition::get_position() const
{
	return _position;
}

void MineChessPosition::_bind_methods()
{
	ClassDB::bind_method(D_METHOD("get_file"), &MineChessPosition::_get_file);
	ClassDB::bind_method(D_METHOD("get_rank"), &MineChessPosition::_get_rank);
	ClassDB::bind_method(D_METHOD("get_index"), &MineChessPosition::_get_index);
	ClassDB::bind_static_method("MineChessPosition", D_METHOD("create", "file", "rank"), &MineChessPosition::_create);

	ADD_PROPERTY(PropertyInfo(Variant::INT, "file", PROPERTY_HINT_ENUM, "a,b,c,d,e,f,g,h"), "", "get_file");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "rank", PROPERTY_HINT_ENUM, "1,2,3,4,5,6,7,8"), "", "get_rank");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "index"), "", "get_index");

	BIND_ENUM_CONSTANT(FILE_A);
	BIND_ENUM_CONSTANT(FILE_B);
	BIND_ENUM_CONSTANT(FILE_C);
	BIND_ENUM_CONSTANT(FILE_D);
	BIND_ENUM_CONSTANT(FILE_E);
	BIND_ENUM_CONSTANT(FILE_F);
	BIND_ENUM_CONSTANT(FILE_G);
	BIND_ENUM_CONSTANT(FILE_H);

	BIND_ENUM_CONSTANT(RANK_1);
	BIND_ENUM_CONSTANT(RANK_2);
	BIND_ENUM_CONSTANT(RANK_3);
	BIND_ENUM_CONSTANT(RANK_4);
	BIND_ENUM_CONSTANT(RANK_5);
	BIND_ENUM_CONSTANT(RANK_6);
	BIND_ENUM_CONSTANT(RANK_7);
	BIND_ENUM_CONSTANT(RANK_8);
}
