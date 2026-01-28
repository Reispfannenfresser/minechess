#pragma once

#include "godot_cpp/classes/ref_counted.hpp"
#include "minechess/board_data.h"
#include "minechess/game.h"
#include "minechess_figure.h"
#include "minechess_position.h"

using namespace godot;

class MineChessGame : public RefCounted, public MineChess::ExplosionResponder, public MineChess::GameOverResponder
{
	GDCLASS(MineChessGame, RefCounted);

public:
	enum EndReason : char
	{
		CHECKMATE = 0,
		STALEMATE,
		INSUFFICIENT_MATERIAL,
		EXPLOSION,
		FAILED_DEFENSE
	};

	enum EndResult : char
	{
		WHITE_WON = 0,
		BLACK_WON,
		DRAW
	};

private:
	MineChess::Game *_game;

public:
	~MineChessGame();
	static Ref<MineChessGame> create(const int &p_mine_count, const bool &p_king_can_defuse, const bool &p_large_explosions, const bool &p_mine_regeneration, const int &p_seed);
	Ref<MineChessFigure> get_figuredata(Ref<MineChessPosition> p_position) const;
	TypedArray<Ref<MineChessFigure>> get_figures() const;
	MineChessFigure::Color get_current_player() const;
	void OnExplosion(const MineChess::Position &p_position) final;
	void OnGameOver(const MineChess::GameEndState &game_end_state) final;
	bool can_undo() const;
	bool can_redo() const;
	void undo();
	void redo();
	bool try_move_figure(Ref<MineChessPosition> p_from, Ref<MineChessPosition> p_to, MineChessFigure::FigureType p_figure_type);
	TypedArray<Ref<MineChessPosition>> get_legal_moves(Ref<MineChessPosition> p_from) const;
	bool is_in_check(Ref<MineChessPosition> position, MineChessFigure::Color color) const;
	bool get_king_can_defuse() const;
	bool get_large_explosions() const;
	bool get_mine_regeneration() const;

protected:
	static void _bind_methods();
};

VARIANT_ENUM_CAST(MineChessGame::EndResult);
VARIANT_ENUM_CAST(MineChessGame::EndReason);
