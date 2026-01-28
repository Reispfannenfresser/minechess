#pragma once

#include <optional>
#include <random>
#include <vector>

namespace MineChess
{

	enum Color : char
	{
		BLACK = 0,
		WHITE
	};

	enum FigureType : char
	{
		PAWN = 0,
		ROOK,
		KNIGHT,
		BISHOP,
		QUEEN,
		KING
	};

	enum File : char
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

	enum Rank : char
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

	enum CastlingType : char
	{
		SHORT_CASTLE = 0,
		LONG_CASTLE
	};

	struct Direction
	{
	public:
		char file_offset;
		char rank_offset;
		bool operator==(const Direction &compare_to) const;
		bool operator!=(const Direction &compare_to) const;
		Direction operator+(const Direction &p_direction) const;
		Direction operator-(const Direction &p_direction) const;
		Direction operator*(char factor) const;
	};

	struct Position
	{
	public:
		File file;
		Rank rank;

		static const std::vector<Direction> neighbour_directions;
		static const std::vector<Direction> rook_directions;
		static const std::vector<Direction> knight_directions;
		static const std::vector<Direction> bishop_directions;

		unsigned char GetIndex() const;
		static std::optional<Position> FromIndex(unsigned char index);
		bool operator==(const Position &compare_to) const;
		bool operator!=(const Position &compare_to) const;
		std::optional<Position> operator+(const Direction &p_direction) const;
		std::optional<Position> operator-(const Direction &p_direction) const;
	};

	struct FigureData
	{
	public:
		FigureType figure_type;
		Color color;
	};

	struct PositionedFigure
	{
	public:
		const Position position;
		const FigureData figure;
	};

	class ExplosionResponder
	{
	public:
		virtual void OnExplosion(const Position &p_position) = 0;
	};

	struct BoardSettings
	{
		const int mine_count;
		const int seed;
		const bool king_can_defuse;
		const bool large_explosions;
		const bool mine_regeneration;
	};

	class BoardData
	{
	private:
		ExplosionResponder &_explosion_responder;
		Color _current_player = Color::WHITE;
		bool _mines[64];
		std::optional<FigureData> _figures[64];
		std::optional<File> _enPassantRights = {};
		bool _castlingRights[2][2] = { { true, true }, { true, true } };
		std::default_random_engine random;
		unsigned char _remaining_mines;

	public:
		const bool king_can_defuse;
		const bool large_explosions;
		const bool mine_regeneration;
		const int mine_count;
		BoardData(const BoardSettings &p_board_settings, ExplosionResponder &p_explosion_responder);

	public:
		Color GetCurrentPlayer() const;
		bool SetCurrentPlayer(const Color &p_player_color);
		void ToggleCurrentPlayer();

		std::optional<FigureData> GetFigure(const Position &p_position) const;
		void GetFigures(std::vector<PositionedFigure> &p_figures) const;
		bool PlaceFigure(const Position &p_position, const FigureData &p_figure_data);
		bool RemoveFigure(const Position &p_position);

		bool IsMine(const Position &p_position) const;
		bool PlaceMine(const Position &p_position);
		std::optional<Position> GetRandomEmptySquare();
		bool RemoveMine(const Position &p_position);
		unsigned char GetMineCount(const Position &p_position) const;
		unsigned char GetRemainingMineCount() const;

		bool GrantCastlingRights(const Color &p_color, const CastlingType &p_castling_type);
		bool RevokeCastlingRights(const Color &p_color, const CastlingType &p_castling_type);
		bool CanCastle(const Color &p_color, const CastlingType &p_castling_type) const;

		bool GrantEnPassantRights(const File &p_file);
		bool RevokeEnPassantRights();
		std::optional<File> GetEnpassantRights() const;

		void CauseExplosion(const Position &p_position);
	};
} //namespace MineChess
