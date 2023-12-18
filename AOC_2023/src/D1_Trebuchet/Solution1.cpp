#include "pch.h"
#include "Solution1.h"

namespace AoC2023_D1 {
	using namespace std;

	// --------------------------- Types ----------------------------------

	static const std::vector<std::string> DIGITS = { "zero", "one", "two", "three", "four", "five", "six", "seven", "eight", "nine" };

	// -------------------------- Functions -------------------------------
	int parseOnlyDigits(const std::string& str) {
		int ns = str.length();
		int i = 0, j = ns - 1;
		while (i < ns && (str[i] < '0' || str[i] > '9')) i++;
		while (j >= 0 && (str[j] < '0' || str[j] > '9')) j--;

		if (i >= ns || j < 0) return 0;
		return static_cast<int>(str[i] - '0') * 10 + static_cast<int>(str[j] - '0');
	}

	int parseStringAndDigits(const std::string& str) {
		int ns = str.length();
		int left_digit = 0;
		bool found = false;
		for (int i = 0; !found && i < ns; i++) {
			if (str[i] >= '0' && str[i] <= '9') {
				left_digit = static_cast<int>(str[i] - '0');
				found = true;
				break;
			}
			for (int index = 0; index < DIGITS.size(); index++) {
				int nd = DIGITS[index].length();
				int j = 0;
				while (i + j < ns && j < nd && str[i + j] == DIGITS[index][j]) j++;
				if (j == nd) {
					left_digit = index;
					found = true;
					break;
				}
			}
		}

		int right_digit = 0;
		found = false;
		for (int i = ns - 1; !found && i >= 0; i--) {
			if (str[i] >= '0' && str[i] <= '9') {
				right_digit = static_cast<int>(str[i] - '0');
				found = true;
				break;
			}
			for (int index = 0; index < DIGITS.size(); index++) {
				int nd = DIGITS[index].length();
				int j = 0;
				while (i - j >= 0 && j < nd && str[i - j] == DIGITS[index][nd - 1 - j]) j++;
				if (j == nd) {
					right_digit = index;
					found = true;
					break;
				}
			}
		}
		return left_digit * 10 + right_digit;
	}

	// --------------------------- Part 1 ---------------------------------
	int solvePart1(const vector<string>& lines) {
		int sum = 0;
		for (auto& line : lines) {
			sum += parseOnlyDigits(line);
		}
		return sum;
	}

	// --------------------------- Part 2 ---------------------------------
	int solvePart2(const vector<string>& lines) {
		int sum = 0;
		for (auto& line : lines) {
			sum += parseStringAndDigits(line);
		}
		return sum;
	}
}

int AoC2023_D1::solve()
{
#if 0 // tests
	auto lines = AoC::readFile("./src/D1_Trebuchet/small.txt");
	if (lines.empty()) return 1;
#else
	auto lines = AoC::readFile("./src/D1_Trebuchet/input.txt");
	if (lines.empty()) return 1;
#endif
	std::ostringstream oss;
	for (auto& line : lines) {
		oss << line << "\r\n";
	}
	oss << "\r\n";
	std::cout << oss.str();
	auto part1 = solvePart1(lines);
	auto part2 = solvePart2(lines);

	std::cout << "-----------------------------------------------------\r\n";
	std::cout << "Part 1 = " << part1 << "\r\n"; // 54388
	std::cout << "Part 2 = " << part2 << "\r\n"; // 53515
	std::cout << "-----------------------------------------------------\r\n";
	return 0;
}
