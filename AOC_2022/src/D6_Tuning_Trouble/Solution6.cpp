#include "pch.h"
#include "Solution6.h"

namespace AoC2022_D6 {
	using namespace std;

	int findMarker(const string& line, int markerSize = 4) {
		string window;
		int idx = 0;
		for (int i = 0; i < line.length(); i++) {
			if (window.size() == markerSize) {
				idx = i;
				break;
			}

			size_t found = window.find(line[i]);
			if (found != std::string::npos)
				window.erase(0, found + 1);

			window += line[i];
		}
		return idx;
	}

	// --------------------------- Part 1 ---------------------------------
	int solvePart1(const string& line) {
		int ans = findMarker(line);
		return ans;
	}

	// --------------------------- Part 2 ---------------------------------
	int solvePart2(const string& line) {
		int ans = findMarker(line, 14);
		return ans;
	}
}


int AoC2022_D6::solve() {
#if 0 // test
	typedef struct test_t { string line; int part1, part2; };
	vector<test_t> tests = {
		{"mjqjpqmgbljsphdztnvjfqwrcgsmlb",7, 19},
		{"bvwbjplbgvbhsrlpgdmjqwftvncz",5, 23},
		{"nppdvjthqldpwncqszvftbrmjlhg",6, 23},
		{"nznrnfrfntjfmvfwmzdfjlvtqnbhcprsg",10, 29},
		{"zcfzfwzzqfrljwzlrfnpqdbhtmscgvjw",11, 26},
	};


	for (auto& test : tests) {
		auto result1 = solvePart1(test.line);
		auto result2 = solvePart2(test.line);
		std::cout << "Part1: " << result1 << " | Part2: " << result2 << " | ";
		std::cout << test.line << "\r\n";
		if (result1 != test.part1 || result2 != test.part2) return 1;
	}

#else
	auto lines = aoc::readFile("./src/D6_Tuning_Trouble/input.txt");
	if (lines.empty()) return 1;

	auto& line = lines.front();

	std::cout << line << std::endl;

	std::cout << "-----------------------------------------------------\r\n";
	std::cout << "Part 1 = " << solvePart1(line) << "\r\n"; // 1794
	std::cout << "Part 2 = " << solvePart2(line) << "\r\n"; // 2851
	std::cout << "-----------------------------------------------------\r\n";
#endif
	return 0;
}