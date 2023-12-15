#include "pch.h"
#include "Solution14.h"

namespace AoC2023_D14 {
	using namespace std;

	// --------------------------- Types ----------------------------------
	typedef struct pos_t {
		int r = -1, c = -1;

		bool operator==(const pos_t& rhs) { return r == rhs.r && c == rhs.c; }
		bool operator!=(const pos_t& rhs) { return !(*this == rhs); }
		pos_t& operator+=(const pos_t& rhs) { r += rhs.r; c += rhs.c; return *this; }
		friend pos_t operator+(pos_t lhs, const pos_t& rhs) { lhs += rhs; return lhs; }
		bool inRange(const int& height, const int& width) { return 0 <= r && r < height && 0 <= c && c < width; }

		friend std::ostream& operator<<(std::ostream& out, const pos_t& pos) {
			return out << "{" << pos.r << ", " << pos.c << "}";
		}
	};

	typedef struct board_t {
		int height = 0, width = 0;
		string grid;

		char at(const pos_t& p) const { return grid[p.r * width + p.c]; }
		char& at(const pos_t& p) { return grid[p.r * width + p.c]; }
		char at(const int& r, const int& c) const { return grid[r * width + c]; }
		char& at(const int& r, const int& c) { return grid[r * width + c]; }

		friend std::ostream& operator<<(std::ostream& out, const board_t& map) {
			std::stringstream ss;
			for (int i = 0; i < map.grid.size(); i++) {
				ss << map.grid[i];
				if ((i + 1) % map.width == 0) {
					ss << "\r\n";
				}
			}
			ss << "\r\n";
			return out << ss.str();
		}
	};

	// ------------------------- Constants --------------------------------
	static constexpr pos_t NORTH = { -1, 0 };
	static constexpr pos_t SOUTH = { 1, 0 };
	static constexpr pos_t EAST = { 0, 1 };
	static constexpr pos_t WEST = { 0, -1 };
	static constexpr array<pos_t, 4> CYCLE = { NORTH, WEST, SOUTH, EAST };

	static constexpr char MOBILE_ROCK = 'O';
	static constexpr char FIXED_ROCK = '#';
	static constexpr char EMPTY = '.';

	static constexpr size_t TEST_ITERATIONS = 1000000000;

	// -------------------------- Functions -------------------------------
	board_t parseData(const vector<string>& lines) {
		board_t board{};
		board.height = 0;
		for (auto& line : lines) {
			std::copy(line.begin(), line.end(), std::back_inserter(board.grid));
			board.height++;
			board.width = line.size();
		}
		return board;
	}

	pos_t nextPosition(pos_t pos, const board_t& board, pos_t dir) {
		bool stop = false;
		while (!stop) {
			auto next = pos + dir;
			if (next.inRange(board.height, board.width) && board.at(next) == EMPTY) {
				pos = next;
			}
			else {
				stop = true;
			}
		}
		return pos;
	}

	board_t tiltBoard(board_t board, pos_t dir = NORTH) {
		if (dir == NORTH || dir == WEST) {
			for (int i = 0; i < board.height; i++) {
				for (int j = 0; j < board.width; j++) {
					pos_t pos = { i, j };
					if (board.at(pos) != MOBILE_ROCK) continue;
					auto next = nextPosition(pos, board, dir);
					if (next != pos)
						std::swap(board.at(pos), board.at(next));
				}
			}
		}
		else if (dir == SOUTH || dir == EAST) {
			for (int i = board.height - 1; i >= 0; i--) {
				for (int j = board.width - 1; j >= 0; j--) {
					pos_t pos = { i, j };
					if (board.at(pos) != MOBILE_ROCK) continue;
					auto next = nextPosition(pos, board, dir);
					if (next != pos)
						std::swap(board.at(pos), board.at(next));
				}
			}
		}
		return board;
	}

	board_t cycleBoard(const board_t& board) {
		board_t output = board;
		for (auto& dir : CYCLE) {
			output = tiltBoard(output, dir);
		}
		return output;
	}

	int64_t calculateLoad(const board_t& board) {
		int64_t load = 0;
		for (int i = 0; i < board.height; i++) {
			for (int j = 0; j < board.width; j++) {
				if (board.at(i, j) == MOBILE_ROCK) {
					load += board.height - i;
				}
			}
		}
		return load;
	}

	// --------------------------- Part 1 ---------------------------------
	int64_t solvePart1(const vector<string>& lines) {
		auto board = parseData(lines);
		auto tiltedBoard = tiltBoard(board);
		auto load = calculateLoad(tiltedBoard);
		std::cout << tiltedBoard;
		return load;
	}

	// --------------------------- Part 2 ---------------------------------
	int64_t solvePart2(const vector<string>& lines) {
		board_t originalBoard = parseData(lines);
		board_t board = originalBoard;

		std::hash<std::string> hasher;
		std::vector<size_t> hashes;
		int repetitionLength = 0, repetitionOffset = 0;

		for (int i = 0; i < TEST_ITERATIONS; i++) {
			board = cycleBoard(board);
			size_t hash = hasher(board.grid);

			auto it = std::find(hashes.begin(), hashes.end(), hash);
			if (it != hashes.end()) {
				int previous_index = std::distance(hashes.begin(), it);
				repetitionLength = hashes.size() - previous_index;
				repetitionOffset = previous_index;
				break;
			}
			else {
				hashes.push_back(hash);
			}
		}

		// back to original board
		board = originalBoard;
		// repetitionOffset: this iterations prepare the board to the beginning of the repetitions
		// (...) % repetitionLength: iterate to a state that's similar to the objective
		int nIterations = ((TEST_ITERATIONS - repetitionOffset) % repetitionLength) + repetitionOffset;
		for (int i = 0; i < nIterations; ++i) {
			board = cycleBoard(board);
		}

		auto load = calculateLoad(board);
		std::cout << board;
		return load;
	}
}

int AoC2023_D14::solve() {
	auto lines = AoC::readFile("./src/D14_Parabolic_Reflector_Dish/input.txt");
	if (lines.empty()) return 1;

	std::ostringstream oss;
	for (auto& line : lines) {
		oss << line << "\r\n";
	}
	oss << "\r\n";
	cout << oss.str();

	auto part1 = solvePart1(lines);
	auto part2 = solvePart2(lines);

	std::cout << "-----------------------------------------------------\r\n";
	std::cout << "Part 1 = " << part1 << "\r\n"; // 111339
	std::cout << "Part 2 = " << part2 << "\r\n"; // 93736
	std::cout << "-----------------------------------------------------\r\n";

	return 0;
}
