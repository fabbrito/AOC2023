#include "pch.h"
#include "Solution3.h"

namespace AoC_D3 {
	using namespace std;
	class Solution;
	class Part1;
	class Part2;
}

class AoC_D3::Solution {
public:
	using Board = vector<string>;

protected:
	static bool isNumber(const char& c) noexcept {
		return (c >= '0' && c <= '9');
	}

	static bool isSymbol(const char& c) noexcept {
		// considering symbols everything that's not a digit (0-9) nor a dot ('.')
		return (c != '.' && ((c >= ' ' && c < '0') || (c > '9' && c <= '~')));
	}

	static int findNumberLength(const Board& board, int r, int c) noexcept {
		int length = 0;
		while (c + length < board.front().length() && isNumber(board[r][c + length])) {
			length++;
		}
		return length;
	}
};

class AoC_D3::Part1 :public AoC_D3::Solution {
public:
	int sumValidPartsIds(const Board& board) {
		int sum = 0;
		helper(board, 0, 0, sum);
		return sum;
	}
private:
	static pair<int, int> nextPart(const Board& board, int r, int c) noexcept {
		int n = board.size(), m = board.front().length();
		while (r != n) {
			if (isNumber(board[r][c]))
				return { r, c };
			// incrementing 'c' by 1 and dealing with overflow
			r = r + (c + 1) / n;
			c = (c + 1) % m;
		}
		return { n, 0 };
	}

	static bool isValidPart(const Board& board, int r, int c) {
		// directions to check: starting from SE, going clockwise, and stopping at E
		const vector<pair<int, int>> dirs = { {1,1},{1,0},{1,-1},{0,-1},{-1,-1},{-1,0},{-1,1},{0,1} };
		for (auto& [i, j] : dirs) {
			// to avoid out of bounds indexing
			if (r + i < 0 || r + i >= board.size()) continue;
			if (c + j < 0 || c + j >= board.front().size()) continue;
			if (isSymbol(board[r + i][c + j])) {
				return true;
			}
			if (i == 0 && j == 1 && isNumber(board[r + i][c + j])) {
				return isValidPart(board, r, c + 1);
			}
		}
		return false;
	}

	// solve for the sum of valid part ids
	static void helper(const Board& board, int r_start, int c_start, int& sum) {
		auto [r, c] = nextPart(board, r_start, c_start);

		if (r == board.size()) {
			return;
		}

		int length = findNumberLength(board, r, c);
		bool valid = isValidPart(board, r, c);
		if (valid) { // valid part id
			sum += stoi(board[r].substr(c, length));
		}
		//cout << "(" << r << ", " << c << ") - " << board[r].substr(c, length) << " - " << valid << "\r\n";

		// incrementing 'c' by 'length' and dealing with overflow
		if (c + length < board.front().length()) {
			helper(board, r, c + length, sum);
		}
		else {
			helper(board, r + 1, 0, sum);
		}
	}
};


class AoC_D3::Part2 : public AoC_D3::Solution {
public:
	int sumOfGearRatios(const Board& board) {
		int sum = 0;
		helper(board, 0, 0, sum);
		return sum;
	}
private:
	static pair<int, int> nextGear(const Board& board, int r, int c) noexcept {
		int n = board.size(), m = board.front().length();
		while (r != n) {
			if (board[r][c] == '*')
				return { r, c };
			// incrementing 'c' by 1 and dealing with overflow
			r = r + (c + 1) / m;
			c = (c + 1) % m;
		}
		return { n, 0 };
	}

	static int recoverNumber(const Board& board, int r, int c) {
		int start = c, end = c;
		while (start >= 0 && isNumber((board[r][start]))) start--; start++;
		while (end < board.front().length() && isNumber((board[r][end]))) end++; end--;
		return stoi(board[r].substr(start, end + 1 - start));
	}

	static bool isValidGear(const Board& board, int r, int c, int& gear_ratio) {
		// dirs consists of the main directions from 0 to 3 and secondary ones from 4 to 7
		// Main directions: starting from S, going clockwise, and stopping at E
		// Secondary directions: starting from SE, going clockwise, and stopping at NE
		const vector<pair<int, int>> dirs = { {1,0},{0,-1},{-1,0},{0,1},{1,1},{1,-1},{-1,-1},{-1,1} };

		int index_to_ignore = -1;
		int count = 0;

		// Checks the main directions (S,W,N,E) first, then check the secondary ones (SE,SW,NW,NE)
		// if a number is found while going through the main directions, then we need to ignore the
		// secondary directions that might intersect. This is only possible because for each gear
		// there are at most two parts connected and, since they are represented by number, there must
		// be something (not a gear '*') separating them.
		for (int index = 0; count < 2 && index < 8; index++) {
			if (index_to_ignore != -1 && (index == 4 + index_to_ignore || index == 4 + (index_to_ignore + 1) % 4)) continue;
			auto& [i, j] = dirs[index];
			// to avoid out of bounds indexing
			if (r + i < 0 || r + i >= board.size()) continue;
			if (c + j < 0 || c + j >= board.front().size()) continue;
			if (isNumber(board[r + i][c + j])) {
				// only ignore secondary directions if S or N
				if (index == 0 || index == 2) index_to_ignore = index;
				gear_ratio *= recoverNumber(board, r + i, c + j);
				count++;
			}
		}
		return count == 2;
	}

	// solve for the sum of gear ratios
	static void helper(const Board& board, int r_start, int c_start, int& sum) {
		auto [r, c] = nextGear(board, r_start, c_start);

		if (r == board.size()) {
			return;
		}
		int gear_ratio = 1;
		bool valid = isValidGear(board, r, c, gear_ratio);
		// cout << "(" << r << ", " << c << ") - " << board[r][c] << " - " << valid << "\r\n";
		if (valid) { // valid gear
			sum += gear_ratio;
		}
		if (c + 1 < board.front().length()) {
			helper(board, r, c + 1, sum);
		}
		else {
			helper(board, r + 1, 0, sum);
		}
	}
};

int AoC_D3::solve()
{
	auto lines = AoC::readFile("./src/D3_Gear_Ratios/input.txt");
	if (lines.empty()) return 1;

	for (auto& line : lines) {
		cout << line << "\r\n";
	}
	cout << "\r\n";

	Part1 sol1;
	auto sum_ids = sol1.sumValidPartsIds(lines);
	std::cout << "-----------------------------------------------------\r\n";
	std::cout << "Sum of the Ids of Valid Parts = " << sum_ids << "\r\n";
	std::cout << "-----------------------------------------------------\r\n";

	Part2 sol2;
	auto sum_grs = sol2.sumOfGearRatios(lines);

	std::cout << "-----------------------------------------------------\r\n";
	std::cout << "Sum of Valid Gear Ratios = " << sum_grs << "\r\n";
	std::cout << "-----------------------------------------------------\r\n";



	return 0;
}
