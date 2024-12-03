#include "day_1.h"

#include "../common_includes.h"

#include <algorithm>
#include <map>
#include <sstream>
#include <vector>

PROBLEM_CLASS_CPP(1);

void problem_1::solve(const std::string& file_name)
{
	std::ifstream input(file_name);

	if (!input.is_open()) {
		return;
	}

	size_t numLines = 0;
	std::string input_line;
	while (std::getline(input, input_line))
		++numLines;
	input.clear();
	input.seekg(0);

	std::vector<int> leftList(numLines), rightList(numLines);

	auto leftItr = leftList.begin();
	auto rightItr = rightList.begin();
	while (std::getline(input, input_line))
	{
		std::stringstream ss(input_line);
		ss >> *leftItr >> *rightItr;
		leftItr++;
		rightItr++;
	}
	input.close();

	std::sort(leftList.begin(), leftList.end());
	std::sort(rightList.begin(), rightList.end());

	unsigned long long sum = 0;
	for (auto leftItr = leftList.begin(), rightItr = rightList.begin(); leftItr != leftList.end(); ++leftItr, ++rightItr)
	{
		sum += abs(*leftItr - *rightItr);
	}

	std::string answer;
	answer = std::to_string(sum);
	output_answer(answer);
}

void problem_2::solve(const std::string& file_name)
{
	std::ifstream input(file_name);

	if (!input.is_open()) {
		return;
	}

	size_t numLines = 0;
	std::string input_line;
	while (std::getline(input, input_line))
		++numLines;
	input.clear();
	input.seekg(0);

	std::vector<int> leftList(numLines);
	std::map<int, int> rightOccurances;

	auto leftItr = leftList.begin();
	while (std::getline(input, input_line))
	{
		std::stringstream ss(input_line);
		ss >> *leftItr;
		leftItr++;
		int rightNum = 0;
		ss >> rightNum;
		if (auto rightItr = rightOccurances.find(rightNum); rightItr != rightOccurances.end())
			++rightItr->second;
		else
			rightOccurances.emplace(rightNum, 1);
	}
	input.close();

	std::sort(leftList.begin(), leftList.end());

	int lastOccurances = 0;
	int lastNum = -1;
	unsigned long long similarity = 0;
	for (auto leftItr = leftList.begin(); leftItr != leftList.end(); ++leftItr)
	{
		if (lastNum == *leftItr)
		{
			similarity += lastOccurances;
			continue;
		}

		auto rightItr = rightOccurances.find(*leftItr);
		int occurances = rightItr != rightOccurances.end() ? rightItr->first * rightItr->second : 0;
		similarity += occurances;
		lastOccurances = occurances;
		lastNum = *leftItr;
	}

	std::string answer;
	answer = std::to_string(similarity);
	output_answer(answer);
}