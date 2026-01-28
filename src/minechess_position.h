#pragma once

#include "godot_cpp/classes/ref_counted.hpp"
#include "godot_cpp/core/binder_common.hpp"
#include "minechess/board_data.h"

using namespace godot;

struct MineChessPosition : public RefCounted
{
	GDCLASS(MineChessPosition, RefCounted);

public:
	enum File : unsigned char
	{
		FILE_A = 0,
		FILE_B,
		FILE_C,
		FILE_D,
		FILE_E,
		FILE_F,
		FILE_G,
		FILE_H
	};

	enum Rank : unsigned char
	{
		RANK_1 = 0,
		RANK_2,
		RANK_3,
		RANK_4,
		RANK_5,
		RANK_6,
		RANK_7,
		RANK_8
	};

private:
	MineChess::Position _position;

private:
	static Ref<MineChessPosition> _create(File file, Rank rank);
	File _get_file() const;
	Rank _get_rank() const;
	unsigned char _get_index() const;

public:
	static Ref<MineChessPosition> create(const MineChess::Position &p_position);
	static Ref<MineChessPosition> create(MineChess::File file, MineChess::Rank rank);
	MineChess::Position get_position() const;

protected:
	static void _bind_methods();
};

VARIANT_ENUM_CAST(MineChessPosition::File);
VARIANT_ENUM_CAST(MineChessPosition::Rank);
