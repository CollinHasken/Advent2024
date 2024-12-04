#include "day_4.h"

#include "../common_includes.h"

#include <vector>

class WordSearch
{
public:
	enum class Direction
	{
		Left,
		TopLeft,
		Top,
		TopRight,
		Right,
		BottomRight,
		Bottom,
		BottomLeft,

		LoopBegin = Left,
		LoopEnd = BottomLeft
	};

	class ConstIterator
	{
	public:
		ConstIterator(size_t col, size_t row, const std::vector<std::string>& words) : m_col(col), m_row(row), m_words(words) {}
		ConstIterator operator++()
		{
			if (++m_col == m_words[0].size())
			{
				m_col = 0;
				++m_row;
			}
			return *this;
		}
		ConstIterator operator+(Direction dir) const
		{
			size_t col = m_col, row = m_row;
			switch (dir)
			{
			case WordSearch::Direction::Left:
			case WordSearch::Direction::TopLeft:
			case WordSearch::Direction::BottomLeft:
				if (m_col == 0)
					return ConstIterator(m_words[0].size(), m_words.size(), m_words);
				--col;
				break;
			case WordSearch::Direction::TopRight:
			case WordSearch::Direction::Right:
			case WordSearch::Direction::BottomRight:
				if (m_col == m_words[0].size() - 1)
					return ConstIterator(m_words[0].size(), m_words.size(), m_words);
				++col;
				break;
			default:
				break;
			}
			switch (dir)
			{
			case WordSearch::Direction::TopLeft:
			case WordSearch::Direction::Top:
			case WordSearch::Direction::TopRight:
				if (m_row == 0)
					return ConstIterator(m_words[0].size(), m_words.size(), m_words);
				--row;
				break;
			case WordSearch::Direction::BottomRight:
			case WordSearch::Direction::Bottom:
			case WordSearch::Direction::BottomLeft:
				if (m_row == m_words.size() - 1)
					return ConstIterator(m_words[0].size(), m_words.size(), m_words);
				++row;
				break;
			default:
				break;
			}
			return ConstIterator(col, row, m_words);
		}
		bool operator==(const ConstIterator& other) const { return !(*this != other); }
		bool operator!=(const ConstIterator& other) const { return m_col != other.m_col && m_row != other.m_row; }
		char operator*() const { return m_words[m_row][m_col]; }

		size_t m_col;
		size_t m_row;
		const std::vector<std::string>& m_words;
	};

	ConstIterator begin() const { return ConstIterator(0, 0, m_words); }
	ConstIterator end() const { return ConstIterator(m_cols, m_rows, m_words); }

	WordSearch(std::ifstream& input)
	{
		std::string inputLine;
		while (std::getline(input, inputLine)) {
			m_words.emplace_back(inputLine);
		}

		m_rows = m_words.size();
		if (!m_rows)
			return;

		m_cols = m_words[0].size();
	}

	template<typename F>
	unsigned long long CountAllMatches(F&& func)
	{
		unsigned long long sum = 0;
		for (auto letterItr = begin(); letterItr != end(); ++letterItr)
		{
			sum += func(letterItr);
		}
		return sum;
	}

private:
	std::vector<std::string> m_words;
	size_t m_cols = 0;
	size_t m_rows = 0;
};

template<> template<>
void Problem<4>::solve<1>()
{
	std::ifstream input(kInputFileName);

	if (!input.is_open()) {
		return;
	}

	WordSearch search(input);
	WordSearch::ConstIterator endItr = search.end();
	unsigned long long sum = search.CountAllMatches([&endItr](const WordSearch::ConstIterator& itr) -> int
		{
			static constexpr const char* kMatch = "XMAS";
			static constexpr int kMatchLength = sizeof("XMAS") - 1;

			int count = 0;
			if (*itr != 'X')
				return count;

			const auto TestDirection = [&endItr](const WordSearch::ConstIterator& itr, WordSearch::Direction dir, const auto& testRef, int iChar = 1) -> bool
			{
				if (iChar == kMatchLength)
					return true;
				if (itr == endItr || *itr != kMatch[iChar])
					return false;
				return testRef(itr + dir, dir, testRef, iChar + 1);
			};

			for (WordSearch::Direction dir = WordSearch::Direction::LoopBegin; dir <= WordSearch::Direction::LoopEnd; dir = static_cast<WordSearch::Direction>(static_cast<int>(dir) + 1))
			{
				if (TestDirection(itr + dir, dir, TestDirection))
					++count;
			}
			return count;
		});

	input.close();

	std::string answer;
	answer = std::to_string(sum);
	OutputAnswer(answer);
}

template<> template<>
void Problem<4>::solve<2>()
{
	std::ifstream input(kInputFileName);

	if (!input.is_open()) {
		return;
	}

	WordSearch search(input);
	WordSearch::ConstIterator endItr = search.end();
	unsigned long long sum = search.CountAllMatches([&endItr](const WordSearch::ConstIterator& itr) -> int
	{
		static constexpr int kMatch = static_cast<int>('M') << 8 | static_cast<int>('S');
		if (*itr != 'A')
			return 0;

		const auto CornersMatch = [&itr, &endItr](WordSearch::Direction dir1, WordSearch::Direction dir2) -> bool
		{
			auto itr1 = itr + dir1;
			auto itr2 = itr + dir2;

			if (itr1 == endItr || itr2 == endItr)
				return false;

			return (((static_cast<int>(*itr1) << 8 | static_cast<int>(*itr2)) == kMatch)
				|| ((static_cast<int>(*itr2) << 8 | static_cast<int>(*itr1)) == kMatch));
		};
		return CornersMatch(WordSearch::Direction::TopLeft, WordSearch::Direction::BottomRight)
			&& CornersMatch(WordSearch::Direction::TopRight, WordSearch::Direction::BottomLeft) ? 1 : 0;
	});

	input.close();

	std::string answer;
	answer = std::to_string(sum);
	OutputAnswer(answer);
}