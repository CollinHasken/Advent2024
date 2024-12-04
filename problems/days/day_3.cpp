#include "day_3.h"

#include "../common_includes.h"

#include <charconv>
#include <regex>

template<> template<>
void Problem<3>::solve<1>()
{
	std::ifstream input(kInputFileName);

	if (!input.is_open()) {
		return;
	}

	unsigned long long sum = 0;
	static constexpr size_t multLength = sizeof("mul(") - 1;
	std::regex multRegex("mul\\([0-9]{1,3}\\,[0-9]{1,3}\\)");
	std::string inputLine;
	while (std::getline(input, inputLine)) {		
		auto multItrEnd = std::sregex_iterator();
		for (auto multItr = std::sregex_iterator(inputLine.begin(), inputLine.end(), multRegex); multItr != multItrEnd; ++multItr)
		{
			std::string multStr = multItr->str();
			size_t commaPos = multStr.find_first_of(",", multLength);

			std::string_view firstNumStr = std::string_view(multStr).substr(multLength, commaPos - multLength);
			int firstNum = 0;
			auto [remainingStr, ec] = std::from_chars(firstNumStr.data(), firstNumStr.data() + firstNumStr.size(), firstNum);
			if (ec != std::errc())
				continue;

			int secondNum = 0;
			if (std::from_chars(remainingStr + 1, multStr.data() + multStr.size(), secondNum).ec != std::errc())
				continue;

			sum += firstNum * secondNum;
		}
	}

	input.close();

	std::string answer;
	answer = std::to_string(sum);
	OutputAnswer(answer);
}

template<> template<>
void Problem<3>::solve<2>()
{
	std::ifstream input(kInputFileName);

	if (!input.is_open()) {
		return;
	}

	unsigned long long sum = 0;
	static constexpr size_t multLength = sizeof("mul(") - 1;
	static constexpr size_t dontLength = sizeof("don't()") - 1;
	static constexpr size_t doLength = sizeof("do()") - 1;
	std::regex multRegex("mul\\([0-9]{1,3}\\,[0-9]{1,3}\\)|do\\(\\)|don't\\(\\)");
	std::string inputLine;
	bool multEnabled = true;
	while (std::getline(input, inputLine)) {
		auto multItrEnd = std::sregex_iterator();
		for (auto multItr = std::sregex_iterator(inputLine.begin(), inputLine.end(), multRegex); multItr != multItrEnd; ++multItr)
		{
			std::string multStr = multItr->str();

			if (multStr.length() == doLength)
			{
				multEnabled = true;
				continue;
			}

			if (multStr.length() == dontLength)
			{
				multEnabled = false;
				continue;
			}

			if (!multEnabled)
				continue;

			size_t commaPos = multStr.find_first_of(",", multLength);

			std::string_view firstNumStr = std::string_view(multStr).substr(multLength, commaPos - multLength);
			int firstNum = 0;
			auto [remainingStr, ec] = std::from_chars(firstNumStr.data(), firstNumStr.data() + firstNumStr.size(), firstNum);
			if (ec != std::errc())
				continue;

			int secondNum = 0;
			if (std::from_chars(remainingStr + 1, multStr.data() + multStr.size(), secondNum).ec != std::errc())
				continue;

			sum += firstNum * secondNum;
		}
	}

	input.close();

	std::string answer;
	answer = std::to_string(sum);
	OutputAnswer(answer);
}