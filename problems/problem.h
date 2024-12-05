#pragma once

#include <iostream>
#include <string>

#include <windows.h>

template <size_t Day>
class Problem
{
public:
	inline static const std::string kInputFileName = "problems/inputs/input_day_" + std::to_string(Day) + ".txt";

	template <size_t Part>
	void solve() { OutputAnswer("Unimplemented"); };

	void solveProblems()
	{
		solve<1>();
		solve<2>();
	}

	// Output the answer and automatically put it in the clipboard
	void OutputAnswer(const std::string& answer) const
	{
		if (answer.empty())
		{
			return;
		}

		OpenClipboard(0);
		EmptyClipboard();
		HGLOBAL hg = GlobalAlloc(GMEM_MOVEABLE, answer.size() + 1);
		if (!hg)
		{
			CloseClipboard();
			return;
		}
		memcpy(GlobalLock(hg), answer.c_str(), answer.size() + 1);
		GlobalUnlock(hg);
		SetClipboardData(CF_TEXT, hg);
		CloseClipboard();
		GlobalFree(hg);

		std::cout << "Problem " << Day << ": " << answer << "\n";
	}
};