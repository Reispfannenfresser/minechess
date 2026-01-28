#include "board_data.h"
#include "../macros/foreach.h"
#include <optional>
#include <random>
#include <vector>

using namespace MineChess;

const std::vector<Direction> Position::neighbour_directions = {
	Direction{ -1, -1 },
	Direction{ -1, 0 },
	Direction{ -1, 1 },
	Direction{ 0, -1 },
	Direction{ 0, 1 },
	Direction{ 1, -1 },
	Direction{ 1, 0 },
	Direction{ 1, 1 }
};

const std::vector<Direction> Position::rook_directions = {
	Direction{ -1, 0 },
	Direction{ 0, -1 },
	Direction{ 0, 1 },
	Direction{ 1, 0 }
};

const std::vector<Direction> Position::knight_directions = {
	Direction{ -2, -1 },
	Direction{ -2, 1 },
	Direction{ -1, -2 },
	Direction{ -1, 2 },
	Direction{ 1, -2 },
	Direction{ 1, 2 },
	Direction{ 2, -1 },
	Direction{ 2, 1 }
};

const std::vector<Direction> Position::bishop_directions = {
	Direction{ -1, -1 },
	Direction{ -1, 1 },
	Direction{ 1, -1 },
	Direction{ 1, 1 }
};

unsigned char Position::GetIndex() const
{
	return rank * 8 + file;
}

std::optional<Position> Position::FromIndex(unsigned char index)
{
	char file = index % 8;
	char rank = index / 8;

	if (file < 0 || file > 7 || rank < 0 || rank > 7)
	{
		return {};
	}

	return Position{ (File)file, (Rank)rank };
}

bool Position::operator==(const Position &compare_to) const
{
	return compare_to.file == this->file && compare_to.rank == this->rank;
}

bool Position::operator!=(const Position &compare_to) const
{
	return compare_to.file != this->file || compare_to.rank != this->rank;
}

std::optional<Position> Position::operator+(const Direction &p_direction) const
{
	char file = this->file + p_direction.file_offset;
	char rank = this->rank + p_direction.rank_offset;

	if (file < 0 || file > FILE_H || rank < 0 || rank > RANK_8)
	{
		return {};
	}

	return Position{ (File)file, (Rank)rank };
}

std::optional<Position> Position::operator-(const Direction &p_direction) const
{
	char file = this->file - p_direction.file_offset;
	char rank = this->rank - p_direction.rank_offset;

	if (file < 0 || file > FILE_H || rank < 0 || rank > RANK_8)
	{
		return {};
	}

	return Position{ (File)file, (Rank)rank };
}

bool Direction::operator==(const Direction &compare_to) const
{
	return compare_to.file_offset == file_offset && compare_to.rank_offset == rank_offset;
}

bool Direction::operator!=(const Direction &compare_to) const
{
	return compare_to.file_offset != file_offset || compare_to.rank_offset != rank_offset;
}

Direction Direction::operator+(const Direction &p_direction) const
{
	return Direction{ static_cast<char>(file_offset + p_direction.file_offset), static_cast<char>(rank_offset + p_direction.rank_offset) };
}

Direction Direction::operator-(const Direction &p_direction) const
{
	return Direction{ static_cast<char>(file_offset - p_direction.file_offset), static_cast<char>(rank_offset - p_direction.rank_offset) };
}

Direction Direction::operator*(char factor) const
{
	return Direction{ static_cast<char>(this->file_offset * factor), static_cast<char>(this->rank_offset * factor) };
}

BoardData::BoardData(const BoardSettings &p_board_settings, ExplosionResponder &p_explosion_responder) :
		random(p_board_settings.seed),
		king_can_defuse(p_board_settings.king_can_defuse),
		large_explosions(p_board_settings.large_explosions),
		mine_regeneration(p_board_settings.mine_regeneration),
		mine_count(p_board_settings.mine_count),
		_explosion_responder(p_explosion_responder)
{
	for (int i = 0; i < 64; ++i)
	{
		_mines[i] = false;
		_figures[i] = {};
	}

	_figures[0] = FigureData{ ROOK, WHITE };
	_figures[1] = FigureData{ KNIGHT, WHITE };
	_figures[2] = FigureData{ BISHOP, WHITE };
	_figures[3] = FigureData{ QUEEN, WHITE };
	_figures[4] = FigureData{ KING, WHITE };
	_figures[5] = FigureData{ BISHOP, WHITE };
	_figures[6] = FigureData{ KNIGHT, WHITE };
	_figures[7] = FigureData{ ROOK, WHITE };

	_figures[8] = FigureData{ PAWN, WHITE };
	_figures[9] = FigureData{ PAWN, WHITE };
	_figures[10] = FigureData{ PAWN, WHITE };
	_figures[11] = FigureData{ PAWN, WHITE };
	_figures[12] = FigureData{ PAWN, WHITE };
	_figures[13] = FigureData{ PAWN, WHITE };
	_figures[14] = FigureData{ PAWN, WHITE };
	_figures[15] = FigureData{ PAWN, WHITE };

	_figures[48] = FigureData{ PAWN, BLACK };
	_figures[49] = FigureData{ PAWN, BLACK };
	_figures[50] = FigureData{ PAWN, BLACK };
	_figures[51] = FigureData{ PAWN, BLACK };
	_figures[52] = FigureData{ PAWN, BLACK };
	_figures[53] = FigureData{ PAWN, BLACK };
	_figures[54] = FigureData{ PAWN, BLACK };
	_figures[55] = FigureData{ PAWN, BLACK };

	_figures[56] = FigureData{ ROOK, BLACK };
	_figures[57] = FigureData{ KNIGHT, BLACK };
	_figures[58] = FigureData{ BISHOP, BLACK };
	_figures[59] = FigureData{ QUEEN, BLACK };
	_figures[60] = FigureData{ KING, BLACK };
	_figures[61] = FigureData{ BISHOP, BLACK };
	_figures[62] = FigureData{ KNIGHT, BLACK };
	_figures[63] = FigureData{ ROOK, BLACK };

	int mine_count = p_board_settings.mine_count;
	if (mine_count < 0)
	{
		mine_count = 0;
	}
	else if (mine_count > 32)
	{
		mine_count = 32;
	}
	_remaining_mines = mine_count;

	// place the desired number of mines
	for (int i = 16; i < 16 + p_board_settings.mine_count; i++)
	{
		_mines[i] = true;
	}

	// Shuffle mines throughout empty space
	for (int i = 16; i < 48; i++)
	{
		int index = std::uniform_int_distribution<>(i, 47)(random);
		bool tmp = _mines[index];
		_mines[index] = _mines[i];
		_mines[i] = tmp;
	}
}

Color BoardData::GetCurrentPlayer() const
{
	return _current_player;
}

bool BoardData::SetCurrentPlayer(const Color &p_color)
{
	if (_current_player != p_color)
	{
		_current_player = p_color;
		return true;
	}

	return false;
}

void BoardData::ToggleCurrentPlayer()
{
	switch (_current_player)
	{
		case WHITE:
			_current_player = BLACK;
			return;
		case BLACK:
			_current_player = WHITE;
			return;
	}
}

std::optional<FigureData> BoardData::GetFigure(const Position &p_position) const
{
	return _figures[p_position.GetIndex()];
}

void BoardData::GetFigures(std::vector<PositionedFigure> &p_figures) const
{
	for (int i = 0; i < 64; i++)
	{
		std::optional<FigureData> figure = _figures[i];
		if (figure)
		{
			p_figures.push_back(PositionedFigure{ *Position::FromIndex(i), *figure });
		}
	}
}

bool BoardData::PlaceFigure(const Position &p_position, const FigureData &p_figure_data)
{
	unsigned char index = p_position.GetIndex();

	if (_figures[index])
	{
		return false;
	}

	_figures[index] = p_figure_data;
	return true;
}

bool BoardData::RemoveFigure(const Position &p_position)
{
	unsigned char index = p_position.GetIndex();

	if (_figures[index])
	{
		_figures[index] = {};
		return true;
	}

	return false;
}

bool BoardData::IsMine(const Position &p_position) const
{
	return _mines[p_position.GetIndex()];
}

bool BoardData::PlaceMine(const Position &p_position)
{
	unsigned char index = p_position.GetIndex();
	if (_mines[index])
	{
		return false;
	}

	_mines[index] = true;
	_remaining_mines++;
	return true;
}

std::optional<Position> BoardData::GetRandomEmptySquare()
{
	std::vector<unsigned char> positions;

	for (int i = 0; i < 64; i++)
	{
		if (!_mines[i] && !_figures[i])
		{
			positions.push_back(i);
		}
	}

	if (positions.size() == 0)
	{
		return {};
	}

	std::uniform_int_distribution<> distribution(0, positions.size() - 1);
	unsigned char index = distribution(random);
	return Position::FromIndex(positions[index]);
}

bool BoardData::RemoveMine(const Position &p_position)
{
	unsigned char index = p_position.GetIndex();
	if (_mines[index])
	{
		_mines[index] = false;
		_remaining_mines--;
		return true;
	}

	return false;
}

unsigned char BoardData::GetMineCount(const Position &p_position) const
{
	unsigned char mine_count = 0;

	if (_mines[p_position.GetIndex()])
	{
		--mine_count;
	}

	FOREACH(direction, Position::neighbour_directions)
	{
		std::optional<Position> neighbour = p_position + *direction;

		if (neighbour && _mines[neighbour->GetIndex()])
		{
			++mine_count;
		}
	}

	return mine_count;
}

unsigned char BoardData::GetRemainingMineCount() const
{
	return _remaining_mines;
}

bool BoardData::GrantCastlingRights(const Color &p_color, const CastlingType &p_castling_type)
{
	if (_castlingRights[p_color][p_castling_type])
	{
		return false;
	}

	_castlingRights[p_color][p_castling_type] = true;
	return true;
}

bool BoardData::RevokeCastlingRights(const Color &p_color, const CastlingType &p_castling_type)
{
	if (_castlingRights[p_color][p_castling_type])
	{
		_castlingRights[p_color][p_castling_type] = false;
		return true;
	}

	return false;
}

bool BoardData::CanCastle(const Color &p_color, const CastlingType &p_castling_type) const
{
	return _castlingRights[p_color][p_castling_type];
}

bool BoardData::GrantEnPassantRights(const File &p_file)
{
	if (_enPassantRights)
	{
		return false;
	}

	_enPassantRights = p_file;
	return true;
}

bool BoardData::RevokeEnPassantRights()
{
	if (_enPassantRights)
	{
		_enPassantRights = {};
		return true;
	}

	return false;
}

std::optional<File> BoardData::GetEnpassantRights() const
{
	return _enPassantRights;
}

void BoardData::CauseExplosion(const Position &p_position)
{
	_explosion_responder.OnExplosion(p_position);
}
