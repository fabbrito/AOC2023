#include "pch.h"
#include "Solution9.h"

namespace AoC2022_D9 {
	using namespace std;

	// --------------------------- Types ----------------------------------
	typedef struct coord_t {
		int x = 0, y = 0;

		bool operator==(const coord_t& rhs) { return x == rhs.x && y == rhs.y; }
		bool operator!=(const coord_t& rhs) { return !(*this == rhs); }
		coord_t& operator+=(const coord_t& rhs) { x += rhs.x; y += rhs.y; return *this; }
		coord_t& operator-=(const coord_t& rhs) { x -= rhs.x; y -= rhs.y; return *this; }
		friend coord_t operator+(coord_t lhs, const coord_t& rhs) { lhs += rhs; return lhs; }
		friend coord_t operator-(coord_t lhs, const coord_t& rhs) { lhs -= rhs; return lhs; }
		bool inRange(const int& height, const int& width) { return 0 <= x && x < height && 0 <= y && y < width; }
		friend std::ostream& operator<<(std::ostream& out, const coord_t& pos) { return out << "(" << pos.x << ", " << pos.y << ")"; }
	} coord_t;

	typedef struct move_t {
		char name = NULL;
		coord_t delta = { 0,0 };
		int repetitions = 0;
	} move_t;

	typedef struct rope_t {
		rope_t(int size) :size(size), config(size, { 0,0 }) {}
		vector<coord_t> config;
		int size;
	}rope_t;

	typedef struct bridge_t {
		bridge_t(int size) : rope(size), states(1, rope_t(size)) {}
		vector<rope_t> states;
		rope_t rope;
	}bridge_t;

	// ------------------------- Constants --------------------------------
	static constexpr coord_t UP = { 0,1 };
	static constexpr coord_t DOWN = { 0,-1 };
	static constexpr coord_t RIGHT = { 1,0 };
	static constexpr coord_t LEFT = { -1,0 };

	// -------------------------- Functions -------------------------------
	vector<move_t> parseMoves(const vector<string>& lines) {
		vector<move_t> moves;
		for (string line : lines) {
			auto info = aoc::parseString(line, " ");
			coord_t delta = { 0,0 };
			switch (info[0][0])
			{
			case 'U':
				delta = UP;
				break;
			case 'R':
				delta = RIGHT;
				break;
			case 'D':
				delta = DOWN;
				break;
			case 'L':
				delta = LEFT;
				break;
			}
			moves.push_back({ info[0][0], delta, std::stoi(info[1]) });
		}
		return moves;
	}

	bridge_t computeMoves(const vector<move_t>& moves, int ropeSize = 2) {
		bridge_t bridge(ropeSize);
		auto& config = bridge.rope.config;
		auto& states = bridge.states;
		for (auto& move : moves) {
			for (int r = 0; r < move.repetitions; r++) {
				auto& prevConfig = states.back().config;
				config[0] += move.delta;
				for (int i = 1; i < ropeSize; i++) {
					coord_t diff = config[i - 1] - config[i];
					if (std::abs(diff.x) == 2 && diff.y == 0)
						config[i] += { (int)(diff.x / 2), 0 };
					else if (diff.x == 0 && std::abs(diff.y) == 2)
						config[i] += { 0, (int)(diff.y / 2) };
					else if (std::abs(diff.x) == 1 && std::abs(diff.y) == 2)
						config[i] += { diff.x, (int)(diff.y / 2) };
					else if (std::abs(diff.x) == 2 && std::abs(diff.y) == 1)
						config[i] += { (int)(diff.x / 2), diff.y };
					else
						config[i] += { (int)(diff.x / 2), (int)(diff.y / 2) };
				}
				states.push_back(bridge.rope);
			}

		}
		return bridge;
	}

	int countPositionsVisited(bridge_t& bridge) {
		auto& states = bridge.states;
		int count = 0;
		for (int i = 0; i < states.size(); i++) {
			bool unique = true;
			for (int j = 0; j < i; j++) {
				if (states[i].config.back() == states[j].config.back()) {
					unique = false;
					break;
				}
			}
			if (unique) count++;
		}
		return count;
	}

	// --------------------------- Part 1 ---------------------------------
	int64_t solvePart1(const vector<string>& lines) {
		auto headMoves = parseMoves(lines);
		auto bridge = computeMoves(headMoves);
		auto count = countPositionsVisited(bridge);
		return count;
	}

	// --------------------------- Part 2 ---------------------------------
	int64_t solvePart2(const vector<string>& lines) {
		auto headMoves = parseMoves(lines);
		auto bridge = computeMoves(headMoves, 10);
		auto count = countPositionsVisited(bridge);
		return count;
	}
}

int AoC2022_D9::solve() {
#if 0 // smaller test: part 1 = 13
	auto lines = aoc::readFile("./src/D9_Rope_Bridge/small.txt");
	if (lines.empty()) return 1;
	auto test1 = solvePart1(lines);
	assert(test1 == 13);
#elif 0 // bigger test: part 2 = 36
	auto lines = aoc::readFile("./src/D9_Rope_Bridge/mid.txt");
	if (lines.empty()) return 1;
	auto test2 = solvePart2(lines);
	assert(test2 == 36);
#else
	auto lines = aoc::readFile("./src/D9_Rope_Bridge/input.txt");
	if (lines.empty()) return 1;
#endif
	aoc::prettyPrint(lines);

	auto part1 = solvePart1(lines);
	auto part2 = solvePart2(lines);
	std::cout << "-----------------------------------------------------\r\n";
	std::cout << "Part 1 = " << part1 << "\r\n"; // 5981
	std::cout << "Part 2 = " << part2 << "\r\n"; // 2352
	std::cout << "-----------------------------------------------------\r\n";
	return 0;
}
