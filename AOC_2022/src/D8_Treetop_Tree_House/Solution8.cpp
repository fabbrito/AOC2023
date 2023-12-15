#include "pch.h"
#include "Solution8.h"

namespace AoC2022_D8 {
	using namespace std;

	// --------------------------- Types ----------------------------------
	typedef struct pos_t {
		int r = -1, c = -1;

		bool operator==(const pos_t& rhs) { return r == rhs.r && c == rhs.c; }
		bool operator!=(const pos_t& rhs) { return !(*this == rhs); }
		pos_t& operator+=(const pos_t& rhs) { r += rhs.r; c += rhs.c; return *this; }
		pos_t& operator-=(const pos_t& rhs) { r -= rhs.r; c -= rhs.c; return *this; }
		friend pos_t operator+(pos_t lhs, const pos_t& rhs) { lhs += rhs; return lhs; }
		friend pos_t operator-(pos_t lhs, const pos_t& rhs) { lhs -= rhs; return lhs; }
		bool inRange(const int& height, const int& width) { return 0 <= r && r < height && 0 <= c && c < width; }
	} pos_t;

	typedef struct tree_t {
		tree_t(int h) : height(h) {}
		int height;
		bool isVisible = false;
		array<int, 4> scores = { -1,-1,-1,-1 };
	}tree_t;

	typedef struct map_t {
		int height = 0, width = 0;
		vector<tree_t> grid;
		tree_t at(const pos_t& p) const { return grid[p.r * width + p.c]; }
		tree_t& at(pos_t& p) { return grid[p.r * width + p.c]; }
		tree_t at(const int& r, const int& c) const { return grid[r * width + c]; }
		tree_t& at(const int& r, const int& c) { return grid[r * width + c]; }
	} map_t;

	// ------------------------- Constants --------------------------------
	static constexpr pos_t NORTH = { -1, 0 };
	static constexpr pos_t SOUTH = { 1, 0 };
	static constexpr pos_t EAST = { 0, 1 };
	static constexpr pos_t WEST = { 0, -1 };
	static constexpr array<pos_t, 4> CYCLE_DIRS = { NORTH, EAST, SOUTH, WEST };
	enum class DIR :int { NONE = -1, NORTH = 0, EAST = 1, SOUTH = 2, WEST = 3 };

	static constexpr int MIN_HEIGHT = 0;
	static constexpr int MAX_HEIGHT = 9;

	// -------------------------- Functions -------------------------------
	map_t parseData(const vector<string>& lines) {
		map_t map;
		map.height = 0;
		for (auto& line : lines) {
			map.width = line.size();
			std::transform(line.cbegin(), line.cend(), std::back_inserter(map.grid),
				[](const char& c)->tree_t {return tree_t((int)(c - '0')); });
			map.height++;
		}
		return map;
	}

	void computeDirVisible(map_t& map, const vector<pos_t>& edge, pos_t dir) {
		for (pos_t pos : edge) {
			int maxHeight = -1;
			while (pos.inRange(map.height, map.width)) {
				if (map.at(pos).height > maxHeight) {
					maxHeight = map.at(pos).height;
					map.at(pos).isVisible = true;
				}
				pos += dir;
			}
		}
	}

	vector<pos_t> generateEdge(int size, pos_t begin, pos_t sum) {
		vector<pos_t> edge(size, { 0 ,0 });
		std::generate(edge.begin(), edge.end(), [&]()->pos_t {pos_t prev = begin; begin += sum; return prev; });
		return edge;
	}

	int countVisible(map_t& map) {
		auto northEdge = generateEdge(map.width, { 0, 0 }, EAST);
		computeDirVisible(map, northEdge, SOUTH);

		auto eastEdge = generateEdge(map.height, northEdge.back(), SOUTH);
		computeDirVisible(map, eastEdge, WEST);

		auto southEdge = generateEdge(map.width, eastEdge.back(), WEST);
		computeDirVisible(map, southEdge, NORTH);

		auto westEdge = generateEdge(map.width, southEdge.back(), NORTH);
		computeDirVisible(map, westEdge, EAST);

		int count = 0;
		for (int i = 0; i < map.grid.size(); i++) {
			count += (int)map.grid[i].isVisible;
		}
		return count;
	}

	int computeDirectionalScore(map_t& map, pos_t pos, DIR dir) {
		int idx = (int)dir;
		auto next = pos + CYCLE_DIRS[idx];
		if (!next.inRange(map.height, map.width)) { // pos -> at the edge
			map.at(pos).scores[idx] = 0;
			return 0;
		}

		int score = 0;
		while (next.inRange(map.height, map.width)) {
			score++;
			if (map.at(next).height >= map.at(pos).height) break;
			next += CYCLE_DIRS[idx];
		}
		map.at(pos).scores[idx] = score;
		return score;
	}

	int computeScores(map_t& map) {
		int maxScore = -1;
		for (int i = 0; i < map.height; i++) {
			for (int j = 0; j < map.width; j++) {
				int score = 1;
				score *= computeDirectionalScore(map, { i,j }, DIR::NORTH);
				score *= computeDirectionalScore(map, { i,j }, DIR::WEST);
				score *= computeDirectionalScore(map, { i,j }, DIR::SOUTH);
				score *= computeDirectionalScore(map, { i,j }, DIR::EAST);
				maxScore = std::max(maxScore, score);
			}
		}
		return maxScore;
	}

	// --------------------------- Part 1 ---------------------------------
	int64_t solvePart1(const vector<string>& lines) {
		auto map = parseData(lines);
		auto count = countVisible(map);

		/*std::ostringstream oss;
		for (int i = 0; i < map.grid.size(); i++) {
			oss << map.grid[i].isVisible;
			if ((i + 1) % map.width == 0) {
				oss << "\r\n";
			}
		}
		oss << "\r\n";
		std::cout << oss.str();*/

		return count;
	}

	// --------------------------- Part 2 ---------------------------------
	int64_t solvePart2(const vector<string>& lines) {
		auto map = parseData(lines);
		auto maxScore = computeScores(map);

		/*std::ostringstream oss;
		for (int i = 0; i < map.grid.size(); i++) {
			int score = 1;
			for (int j = 0; j < 4; j++)
				score *= map.grid[i].scores[j];
			oss << score << "\t";
			if ((i + 1) % map.width == 0) {
				oss << "\r\n";
			}
		}
		oss << "\r\n";
		std::cout << oss.str();*/

		return maxScore;
	}
}

int AoC2022_D8::solve() {
#if 0 // tests
	auto lines = AoC::readFile("./src/D8_Treetop_Tree_House/small.txt");
	if (lines.empty()) return 1;
#else
	auto lines = AoC::readFile("./src/D8_Treetop_Tree_House/input.txt");
	if (lines.empty()) return 1;
#endif
	std::ostringstream oss;
	for (auto& line : lines) {
		oss << line << "\r\n";
	}
	oss << "\r\n";
	cout << oss.str();

	auto part1 = solvePart1(lines);
	auto part2 = solvePart2(lines);
	std::cout << "-----------------------------------------------------\r\n";
	std::cout << "Part 1 = " << part1 << "\r\n"; // 1715
	std::cout << "Part 2 = " << part2 << "\r\n"; // 374400
	std::cout << "-----------------------------------------------------\r\n";
	return 0;
}
