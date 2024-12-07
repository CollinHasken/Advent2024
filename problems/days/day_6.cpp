#include "day_6.h"

#include "../common_includes.h"

#include <vector>

enum class Direction
{
	None	= 0b0000,
	Up		= 0b0001,
	Right	= 0b0010,
	Down	= 0b0100,
	Left	= 0b1000
};
inline constexpr Direction operator|(Direction x, Direction y)
{
	return static_cast<Direction>(static_cast<int>(x) | static_cast<int>(y));
}
inline Direction& operator|=(Direction& x, Direction y)
{
	x = x | y;
	return x;
}
inline constexpr Direction operator&(Direction x, Direction y)
{
	return static_cast<Direction>(static_cast<int>(x) & static_cast<int>(y));
}
inline Direction& operator&=(Direction& x, Direction y)
{
	x = x & y;
	return x;
}

Direction NextDirection(Direction dir) { return dir == Direction::Left ? Direction::Up : static_cast<Direction>(static_cast<int>(dir) << 1); }

using Position = std::pair<int, int>;

class MapIterator
{
public:
	MapIterator(const std::vector<std::vector<bool>>& obstacles, Direction dir, Position pos, size_t width, size_t height) :
		m_obstacles(obstacles), m_dir(dir), m_pos(pos), m_width(width), m_height(height) {}

	bool IsValid(const Position& pos) const { return pos.first != -1 && pos.first != m_width && pos.second != -1 && pos.second != m_height; }
	operator bool() const { return IsValid(m_pos); }
	const Position& Pos() const { return m_pos; }
	Direction Dir() const { return m_dir; }
	MapIterator operator++()
	{
		Position nextPos = m_pos;
		switch (m_dir)
		{
		case Direction::Up:
			--nextPos.second;
			break;
		case Direction::Right:
			++nextPos.first;
			break;
		case Direction::Down:
			++nextPos.second;
			break;
		case Direction::Left:
			--nextPos.first;
			break;
		default:
			break;
		}

		if (IsValid(nextPos) && m_obstacles[nextPos.second][nextPos.first])
			Rotate();
		else
			m_pos = nextPos;
		return *this;
	}

private:
	void Rotate() { m_dir = NextDirection(m_dir); }
	const std::vector<std::vector<bool>>& m_obstacles;
	Direction m_dir;
	Position m_pos;
	size_t m_width;
	size_t m_height;
};

template<> template<>
void Problem<6>::solve<1>()
{
	std::ifstream input(kInputFileName);

	if (!input.is_open()) {
		return;
	}

	std::vector<std::vector<bool>> obstacles;
	obstacles.reserve(150);
	std::vector<std::vector<bool>> traveled;
	traveled.reserve(150);
	std::string inputLine;
	Position pos;
	int iLine = 0;
	while (std::getline(input, inputLine)) {
		obstacles.push_back(std::vector<bool>(inputLine.size()));
		traveled.push_back(std::vector<bool>(inputLine.size()));
		for (int iChar = 0; iChar < inputLine.size(); ++iChar)
		{
			if (inputLine[iChar] == '#')
				obstacles[iLine][iChar] = true;
			else if (inputLine[iChar] == '^')
				pos = { iChar, iLine };
		}
		++iLine;
	}

	const size_t width = obstacles[0].size();
	const size_t height = obstacles.size();
	Direction curDirection = Direction::Up;	
	for (MapIterator itr(obstacles, curDirection, pos, width, height); itr; ++itr)
	{
		const Position& pos = itr.Pos();
		traveled[pos.second][pos.first] = true;
	}

	unsigned long long sum = 0;
	for (const std::vector<bool>& row : traveled) {
		sum += std::count(row.begin(), row.end(), true);
	}

	input.close();

	std::string answer;
	answer = std::to_string(sum);
	OutputAnswer(answer);
}

template<> template<>
void Problem<6>::solve<2>()
{
	std::ifstream input(kInputFileName);

	if (!input.is_open()) {
		return;
	}

	std::vector<std::vector<bool>> obstacles;
	obstacles.reserve(150);
	std::vector<std::vector<Direction>> traveled;
	traveled.reserve(150);
	std::vector<std::vector<bool>> newObstacles;
	newObstacles.reserve(150);
	std::string inputLine;
	Position startPos;
	int iLine = 0;
	while (std::getline(input, inputLine)) {
		obstacles.push_back(std::vector<bool>(inputLine.size()));
		newObstacles.push_back(std::vector<bool>(inputLine.size()));
		traveled.push_back(std::vector<Direction>(inputLine.size()));
		for (int iChar = 0; iChar < inputLine.size(); ++iChar)
		{
			if (inputLine[iChar] == '#')
				obstacles[iLine][iChar] = true;
			else if (inputLine[iChar] == '^')
				startPos = { iChar, iLine };
		}
		++iLine;
	}
	input.close();

	std::vector<std::vector<Direction>> testTraveled;
	const size_t width = obstacles[0].size();
	const size_t height = obstacles.size();
	for (MapIterator itr(obstacles, Direction::Up, startPos, width, height); itr;)
	{
		const Position pos = itr.Pos();
		const Direction dir = itr.Dir();
		traveled[pos.second][pos.first] |= itr.Dir();
		++itr;

		// If we didn't reach the end and didn't hit an existing obstacle
		const Position& newObstocalPos = itr.Pos();
		if (itr && dir == itr.Dir() && newObstocalPos != startPos && (traveled[newObstocalPos.second][newObstocalPos.first] == Direction::None))
		{
			testTraveled = traveled;
			obstacles[newObstocalPos.second][newObstocalPos.first] = true;
			const Direction startLoopDir = NextDirection(dir);
			testTraveled[pos.second][pos.first] |= startLoopDir;
			for (MapIterator loopItr = ++MapIterator(obstacles, startLoopDir, pos, width, height); loopItr; ++loopItr)
			{
				const Direction loopDir = loopItr.Dir();
				const Position& loopPos = loopItr.Pos();
				if ((testTraveled[loopPos.second][loopPos.first] & loopDir) != Direction::None)
				{
					newObstacles[newObstocalPos.second][newObstocalPos.first] = true;
					break;
				}
				testTraveled[loopPos.second][loopPos.first] |= loopDir;
			}
			obstacles[newObstocalPos.second][newObstocalPos.first] = false;
		}
	}

	unsigned long long sum = 0;
	for (const std::vector<bool>& row : newObstacles) {
		sum += std::count(row.begin(), row.end(), true);
	}

	std::string answer;
	answer = std::to_string(sum);
	OutputAnswer(answer);
}