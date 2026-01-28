#pragma once

#include "board_data.h"
#include "move.h"
#include <stack>

namespace MineChess
{
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

	struct GameEndState
	{
	public:
		EndReason reason;
		EndResult result;
	};

	class GameOverResponder
	{
	public:
		virtual void OnGameOver(const GameEndState &state) = 0;
	};

	class Game
	{
	private:
		BoardData _board;

		GameOverResponder &_game_over_responder;

		std::stack<Move *> _undo_stack;
		std::stack<Move *> _redo_stack;

	private:
		void PerformMove(Move *move);
		void HandleGameOver();
		void EndGame(EndReason p_reason, EndResult p_result);
		std::optional<GameEndState> _game_over_state = {};

	public:
		Game(const BoardSettings &board_settings, ExplosionResponder &p_explosion_responder, GameOverResponder &p_game_over_responder);
		~Game();
		bool IsGameOver() const;
		std::optional<GameEndState> GetGameEndState() const;
		Color GetCurrentPlayer() const;
		int GetMineCount(const Position &p_position) const;
		std::optional<FigureData> GetFigure(const Position &p_position) const;
		void GetFigures(std::vector<PositionedFigure> &figures) const;
		bool TryMoveFigure(const Position &p_from, const Position &p_to);
		bool TryMoveFigure(const Position &p_from, const Position &p_to, FigureType p_figure_type);
		void GetLegalMoves(const Position &p_from, std::vector<Position> &p_strong_legal_moves);
		bool IsInCheck(Position position, Color color) const;
		void Undo();
		bool CanUndo() const;
		void Redo();
		bool CanRedo() const;
		bool HasLargeExplosions() const;
		bool CanKingDefuse() const;
		bool DoMinesRegenerate() const;
	};
} //namespace MineChess
