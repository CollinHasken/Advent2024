#include "day_2.h"

#include "../common_includes.h"

#include <sstream>
#include <vector>

PROBLEM_CLASS_CPP(2);

enum class LevelDirection
{
	Unknown,
	Increase,
	Decrease
};

void problem_1::solve(const std::string& file_name)
{
	std::ifstream input(file_name);

	if (!input.is_open()) {
		return;
	}

	unsigned long long safeReports = 0;
	std::string input_line;
	while (std::getline(input, input_line)) {
		std::stringstream ss(input_line);
		bool isSafe = true;
		LevelDirection curDirection = LevelDirection::Unknown;
		int curLevel = -1;
		int nextLevel;
		while (ss >> nextLevel)
		{
			if (curLevel != -1)
			{
				const int levelDif = nextLevel - curLevel;
				const int levelDifAbs = abs(levelDif);
				const LevelDirection levelDirection = levelDif > 0 ? LevelDirection::Increase : (levelDif < 0 ? LevelDirection::Decrease : LevelDirection::Unknown);
				
				isSafe = levelDifAbs >= 1 && levelDifAbs <= 3;					
				if (curDirection == LevelDirection::Unknown)
					curDirection = levelDirection;
				else if (curDirection != levelDirection)
					isSafe = false;

				if (!isSafe)
					break;
			}
			curLevel = nextLevel;
		}
		if (isSafe)
			++safeReports;
	}

	input.close();

	std::string answer;
	answer = std::to_string(safeReports);
	output_answer(answer);
}

bool isReportSafe(const std::vector<int>& levels, size_t unsafeLevels = 0, int skipLevel = -1)
{
	static constexpr size_t kMaxUnsafeLevels = 2;
	LevelDirection curDirection = LevelDirection::Unknown;
	for (size_t lLevel = 0, rLevel = 1; rLevel < levels.size(); ++rLevel)
	{
		if (rLevel == skipLevel)
			continue;
		if (lLevel == skipLevel)
			if (++lLevel == rLevel)
				++rLevel;

		const int levelDif = levels[rLevel] - levels[lLevel];
		const int levelDifAbs = abs(levelDif);
		const LevelDirection levelDirection = levelDif > 0 ? LevelDirection::Increase : (levelDif < 0 ? LevelDirection::Decrease : LevelDirection::Unknown);

		if (curDirection == LevelDirection::Unknown)
			curDirection = levelDirection;

		if (levelDifAbs < 1 || levelDifAbs > 3 || curDirection != levelDirection)
		{
			++unsafeLevels;
			if (unsafeLevels == kMaxUnsafeLevels)
				return false;

			// For the 3rd number, we need to also check if skipping the first works
			if (rLevel == 2)
			{
				// Skip first
				if (isReportSafe(levels, unsafeLevels, 0))
					return true;
			}

			// Try to skip the current number
			if (isReportSafe(levels, unsafeLevels, rLevel))
				return true;

			// Try to skip the previous number
			return isReportSafe(levels, unsafeLevels, lLevel);
		}

		++lLevel;
	}
	return true;
}

void problem_2::solve(const std::string& file_name)
{
	std::ifstream input(file_name);

	if (!input.is_open()) {
		return;
	}
	
	unsigned long long safeReports = 0;
	std::string input_line;
	while (std::getline(input, input_line)) {
		std::stringstream ss(input_line);

		std::vector<int> levels;
		int level;
		while (ss >> level)
			levels.push_back(level);
				
		if (isReportSafe(levels))
			++safeReports;
	}

	input.close();

	std::string answer;
	answer = std::to_string(safeReports);
	output_answer(answer);
}