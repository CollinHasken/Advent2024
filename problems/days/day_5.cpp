#include "day_5.h"

#include "../common_includes.h"

#include <bitset>
#include <map>
#include <vector>

using PageBits = std::bitset<100>;
using PageRules = std::map<unsigned short, PageBits>;

// Map each page to a bitset of pages that it must come before
void ReadRules(PageRules& rules, std::ifstream& input)
{
	std::string inputLine;
	while (std::getline(input, inputLine)) {
		if (inputLine.empty())
			break;

		size_t delimPos = inputLine.find_first_of("|");
		std::string_view firstNumStr = std::string_view(inputLine).substr(0, delimPos);
		int firstNum = 0;
		auto [remainingStr, ec] = std::from_chars(firstNumStr.data(), firstNumStr.data() + firstNumStr.size(), firstNum);
		if (ec != std::errc())
			continue;

		int secondNum = 0;
		if (std::from_chars(remainingStr + 1, inputLine.data() + inputLine.size(), secondNum).ec != std::errc())
			continue;

		auto [itr, wasInserted] = rules.try_emplace(firstNum, 0);
		itr->second.set(secondNum);
	}
}

/*
 * Map each page to a bitset of pages that it must come before
 * Create a mask of pages seen as we go and compare with the current page's rule
 * If any bit is set, then that means a page has appeared before that shouldn't
 */
template<> template<>
void Problem<5>::solve<1>()
{
	std::ifstream input(kInputFileName);

	if (!input.is_open()) {
		return;
	}

	PageRules rules;
	ReadRules(rules, input);

	unsigned long long sum = 0;
	std::vector<int> pages;
	pages.reserve(20);
	std::string inputLine;
	while (std::getline(input, inputLine)) {
		std::stringstream ss(inputLine);

		PageBits currentRule;
		pages.clear();

		char delim;
		int page;
		bool isFirst = true;
		bool isValid = true;
		while (ss >> page)
		{
			if (auto itr = rules.find(page); itr != rules.end())
			{
				if ((currentRule & itr->second).any())
				{
					isValid = false;
					break;
				}
			}
			currentRule.set(page);
			pages.push_back(page);

			ss >> delim;
		}

		if (isValid)
			sum += pages[pages.size() / 2];
	}

	input.close();

	std::string answer;
	answer = std::to_string(sum);
	OutputAnswer(answer);
}

/*
 * Map each page to a bitset of pages that it must come before
 * Create a mask of pages seen as we go and compare with the current page's rule
 * If any bit is set, then that means a page has appeared before that shouldn't
 * Loop backwards, removing the pagess from our mask, until the mask satifies the current page's rule
 * Insert the page in this location
 */
template<> template<>
void Problem<5>::solve<2>()
{
	std::ifstream input(kInputFileName);

	if (!input.is_open()) {
		return;
	}

	PageRules rules;
	ReadRules(rules, input);

	unsigned long long sum = 0;
	std::vector<int> pages;
	pages.reserve(20);
	std::string inputLine;
	while (std::getline(input, inputLine)) {
		std::stringstream ss(inputLine);

		PageBits currentRule;
		pages.clear();

		char delim;
		int page;
		bool isInvalid = false;
		bool isFirst = true;
		while (ss >> page)
		{
			bool wasInserted = false;
			if (auto itr = rules.find(page); itr != rules.end())
			{
				const PageBits& pageRule = itr->second;
				if ((currentRule & pageRule).any())
				{
					isInvalid = true;
					PageBits testRule = currentRule;
					for (auto rItr = pages.rbegin(); rItr != pages.rend(); ++rItr)
					{
						testRule.reset(*rItr);
						if ((testRule & pageRule).none())
						{
							pages.insert(rItr.base() - 1, page);
							wasInserted = true;
							break;
						}
					}
				}
			}
			currentRule.set(page);
			if (!wasInserted)
				pages.push_back(page);

			ss >> delim;
		}

		if (isInvalid)
			sum += pages[pages.size() / 2];
	}

	input.close();

	std::string answer;
	answer = std::to_string(sum);
	OutputAnswer(answer);
}