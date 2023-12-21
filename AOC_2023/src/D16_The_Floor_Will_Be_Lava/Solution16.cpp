#include "pch.h"
#include "Solution16.h"

namespace AoC2023_D16 {
	using namespace std;

	// --------------------------- Types ----------------------------------
	typedef struct pos_t {
		int r = -1, c = -1;

		bool operator==(const pos_t& rhs) { return r == rhs.r && c == rhs.c; }
		bool operator!=(const pos_t& rhs) { return !(*this == rhs); }
		bool operator==(const pos_t& rhs) const { return r == rhs.r && c == rhs.c; }
		bool operator!=(const pos_t& rhs) const { return !(*this == rhs); }
		pos_t& operator+=(const pos_t& rhs) { r += rhs.r; c += rhs.c; return *this; }
		friend pos_t operator+(pos_t lhs, const pos_t& rhs) { lhs += rhs; return lhs; }
		pos_t& operator-=(const pos_t& rhs) { r -= rhs.r; c -= rhs.c; return *this; }
		friend pos_t operator-(pos_t lhs, const pos_t& rhs) { lhs -= rhs; return lhs; }

		bool inRange(const int& height, const int& width) { return 0 <= r && r < height && 0 <= c && c < width; }

		friend std::ostream& operator<<(std::ostream& out, const pos_t& pos) { return out << "(" << pos.r << ", " << pos.c << ")"; }
	};

	enum class DIRECTION :int { NONE = -1, UP, LEFT, DOWN, RIGHT };
	static constexpr pos_t UP = { -1,0 };
	static constexpr pos_t DOWN = { 1,0 };
	static constexpr pos_t LEFT = { 0,-1 };
	static constexpr pos_t RIGHT = { 0,1 };
	static constexpr array<pos_t, 4> DIRECTIONS = { UP,LEFT,DOWN,RIGHT };
	static constexpr array<char, 4> DIRECTION_IDS = { 'U','L','D','R' };

	typedef struct ray_t {
		ray_t(pos_t pos, DIRECTION dir) :pos(pos), direction(dir) {}
		pos_t pos;
		DIRECTION direction = DIRECTION::NONE;

		void advance() { pos = pos + DIRECTIONS[(int)direction]; }
		string toString() { return std::to_string(pos.r) + ',' + std::to_string(pos.c) + DIRECTION_IDS[(int)direction]; }

		bool operator==(const ray_t& rhs) { return pos == rhs.pos && direction == rhs.direction; }
		bool operator!=(const ray_t& rhs) { return !(*this == rhs); }
		bool operator==(const ray_t& rhs) const { return pos == rhs.pos && direction == rhs.direction; }
		bool operator!=(const ray_t& rhs) const { return !(*this == rhs); }
		friend std::ostream& operator<<(std::ostream& out, const ray_t& ray) { return out << ray.pos << ", " << (int)ray.direction; }
	}ray_t;

	typedef struct map_t {
		map_t(const vector<string>& lines) : grid(lines.size(), vector<char>()) {
			std::for_each(lines.begin(), lines.end(),
				[&](const string& line) {
					std::copy(line.begin(), line.end(), std::back_inserter(grid[height]));
					width = line.size(), height++;
				});
		}

		vector<vector<char>> grid = {};
		int height = 0, width = 0;

		char operator[](const pos_t& pos) const { return grid[pos.r][pos.c]; }
		char& operator[](const pos_t& pos) { return grid[pos.r][pos.c]; }
	}map_t;

	// ------------------------- Constants --------------------------------
	static constexpr int TOTAL_DIRECTIONS = 4;

	// -------------------------- Functions -------------------------------

	vector<vector<bool>> energize(map_t& map, ray_t orig) {
		vector<ray_t> currRays = { }, nextRays = { orig };
		vector<ray_t> splitters;
		vector<vector<bool>> energizedGrid(map.height, vector<bool>(map.width, false));
		while (nextRays.size() > 0) {
			currRays = nextRays;
			nextRays.clear();
			for (auto& ray : currRays) {
				energizedGrid[ray.pos.r][ray.pos.c] = true;

				auto it = std::find(splitters.begin(), splitters.end(), ray);
				if (it != splitters.end()) { // ray will colide with already computed splitter
					continue;
				}

				if (map[ray.pos] == '\\') { // deflection
					if (ray.direction == DIRECTION::LEFT || ray.direction == DIRECTION::RIGHT) // -90 deg
						ray.direction = (DIRECTION)(((int)ray.direction + 3) % TOTAL_DIRECTIONS);
					else if (ray.direction == DIRECTION::UP || ray.direction == DIRECTION::DOWN) // +90 deg
						ray.direction = (DIRECTION)(((int)ray.direction + 1) % TOTAL_DIRECTIONS);
				}
				else if (map[ray.pos] == '/') { // deflection
					if (ray.direction == DIRECTION::LEFT || ray.direction == DIRECTION::RIGHT) // +90 deg
						ray.direction = (DIRECTION)(((int)ray.direction + 1) % TOTAL_DIRECTIONS);
					else if (ray.direction == DIRECTION::UP || ray.direction == DIRECTION::DOWN) // -90 deg
						ray.direction = (DIRECTION)(((int)ray.direction + 3) % TOTAL_DIRECTIONS);
				}
				// DIRECTION::LEFT || RIGHT with horizontal splitter ('-') -> do nothing
				else if (map[ray.pos] == '-' && (ray.direction == DIRECTION::UP || ray.direction == DIRECTION::DOWN)) {
					// add splitter position in both perpendicular directions
					splitters.push_back(ray);
					splitters.emplace_back(ray_t(ray.pos, (DIRECTION)(((int)ray.direction + 2) % TOTAL_DIRECTIONS)));
					// spawn next rays
					nextRays.push_back(ray_t(ray.pos, DIRECTION::LEFT)); // spawn ray moving to the LEFT
					nextRays.push_back(ray_t(ray.pos, DIRECTION::RIGHT)); // spawn ray moving to the RIGHT
					continue; // compute moves next round
				}
				// DIRECTION::UP || DOWN with vertical splitter ('|') -> do nothing
				else if (map[ray.pos] == '|' && (ray.direction == DIRECTION::LEFT || ray.direction == DIRECTION::RIGHT)) {
					// add splitter position in both perpendicular directions
					splitters.push_back(ray);
					splitters.emplace_back(ray_t(ray.pos, (DIRECTION)(((int)ray.direction + 2) % TOTAL_DIRECTIONS)));
					// spawn next rays
					nextRays.push_back(ray_t(ray.pos, DIRECTION::UP)); // spawn ray moving to the UP
					nextRays.push_back(ray_t(ray.pos, DIRECTION::DOWN)); // spawn ray moving to the DOWN
					continue; // compute moves next round
				}

				ray.advance(); // move ray
				if (ray.pos.inRange(map.height, map.width)) {
					nextRays.push_back(ray);
				}
			}
		}
		return energizedGrid;
	}

	int countEnergized(const vector<vector<bool>>& energizedGrid, int height, int width) {
		int count = 0;
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				count += (int)energizedGrid[i][j];
			}
		}
		return count;
	}

	vector<ray_t> generateEntries(int height, int width) {
		vector<ray_t> entries;
		// rays entering from the TOP edge and moving DOWN
		for (int j = 0; j < width; j++)
			entries.emplace_back(ray_t({ 0, j }, DIRECTION::DOWN));
		// rays entering from the BOTTOM edge and moving UP
		for (int j = 0; j < width; j++)
			entries.emplace_back(ray_t({ height - 1, j }, DIRECTION::UP));
		// rays entering from the LEFT edge and moving RIGHT
		for (int i = 0; i < height; i++)
			entries.emplace_back(ray_t({ i, 0 }, DIRECTION::RIGHT));
		// rays entering from the RIGHT edge and moving LEFT
		for (int i = 0; i < height; i++)
			entries.emplace_back(ray_t({ i, width - 1 }, DIRECTION::LEFT));
		return entries;
	}

	// --------------------------- Part 1 ---------------------------------
	int solvePart1(const vector<string>& lines) {
		map_t map(lines);
		ray_t ray({ 0,0 }, DIRECTION::RIGHT);
		auto grid = energize(map, ray);
		return countEnergized(grid, map.height, map.width);
	}

	// --------------------------- Part 2 ---------------------------------
	int solvePart2(const vector<string>& lines) {
		map_t map(lines);
		auto rays = generateEntries(map.height, map.width);
		int maxEnergized = -1;
		for (auto& ray : rays) {
			auto grid = energize(map, ray);
			auto count = countEnergized(grid, map.height, map.width);
			maxEnergized = std::max(maxEnergized, count);
		}
		return maxEnergized;
	}
}

int AoC2023_D16::solve() {
#if 1 // tests
	auto lines = aoc::readFile("./src/D16_The_Floor_Will_Be_Lava/small.txt");
	if (lines.empty()) return 1;

	// pretty print
	std::ostringstream oss;
	int n = lines.size(), m = lines.front().size();
	oss << "  ";
	for (int j = 0; j < m; j++) {
		oss << j << " ";
	}
	oss << "\r\n";
	for (int i = 0; i < n; i++) {
		oss << i << " ";
		for (int j = 0; j < m; j++) {
			oss << lines[i][j];
			if (j < lines[i].length() - 1)
				oss << " ";
			else
				oss << "\r\n";
		}
	}
	oss << "\r\n";
	cout << oss.str();

	auto part1 = solvePart1(lines);
	assert(part1 == 46);

	auto part2 = solvePart2(lines);
	assert(part2 == 51);
#else
	auto lines = aoc::readFile("./src/D16_The_Floor_Will_Be_Lava/input.txt");
	if (lines.emptyInterval()) return 1;

	std::ostringstream oss;
	for (auto& line : lines) {
		oss << line << "\r\n";
	}
	oss << "\r\n";
	cout << oss.str();

	auto part1 = solvePart1(lines);
	auto part2 = solvePart2(lines);
#endif

	std::cout << "-----------------------------------------------------\r\n";
	std::cout << "Part 1 = " << part1 << "\r\n"; // 7884
	std::cout << "Part 2 = " << part2 << "\r\n"; // 8185
	std::cout << "-----------------------------------------------------\r\n";
	return 0;
	}
