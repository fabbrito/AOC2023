#include "pch.h"
#include "Solution18.h"

namespace AoC2023_D18 {
	using namespace std;

	// --------------------------- Types ----------------------------------

	// (x,y) coordinates -> x = row, y = col
	typedef struct pos_t {
		long long x = 0, y = 0;

		bool operator==(const pos_t& rhs) const { return x == rhs.x && y == rhs.y; }
		bool operator!=(const pos_t& rhs) const { return !(*this == rhs); }
		pos_t& operator+=(const pos_t& rhs) { x += rhs.x; y += rhs.y; return *this; }
		friend pos_t operator+(pos_t lhs, const pos_t& rhs) { lhs += rhs; return lhs; }
		pos_t& operator-=(const pos_t& rhs) { x -= rhs.x; y -= rhs.y; return *this; }
		friend pos_t operator-(pos_t lhs, const pos_t& rhs) { lhs -= rhs; return lhs; }
		friend pos_t operator*(long long lhs, pos_t rhs) { rhs.x *= lhs; rhs.y *= lhs; return rhs; }

		struct hash {
			size_t operator()(const pos_t& lhs) const { return std::hash<long long>()(lhs.x) ^ std::hash<long long>()(lhs.y); }
		};
	};

	enum class DIRECTION :int { NONE = -1, EAST, SOUTH, WEST, NORTH};
	static constexpr pos_t EAST = { 0,1 };
	static constexpr pos_t SOUTH = { 1,0 };
	static constexpr pos_t WEST = { 0,-1 };
	static constexpr pos_t NORTH = { -1,0 };
	static constexpr array<pos_t, 4> DIRECTIONS = { EAST,SOUTH,WEST,NORTH };
	static constexpr int TOTAL_DIRECTIONS = 4;

	struct path_t {
		vector<pos_t> vertices = {};
		long long boundaryCount = 0;
	};

	// -------------------------- Functions -------------------------------
	path_t createPath(const vector<string>& lines, pos_t begin = { 0,0 }, bool swapCodes = false) {
		const string MOVES = "RDLU";
		path_t path;
		pos_t pos = begin;
		pos_t delta = { 0,0 };
		long long length = 1;
		for (string line : lines) {
			auto info = aoc::parseString(line, " ", true);
			if (!swapCodes) {
				auto moveIdx = MOVES.find(info[0]);
				if (moveIdx == std::string::npos) break;
				delta = DIRECTIONS[moveIdx];
				length = std::stoll(info[1]);
			}
			else {
				length = std::stoll(info[2].substr(2, 5), nullptr, 16);
				auto moveIdx = static_cast<size_t>(info[2][7]-'0');
				if (moveIdx == std::string::npos) break;
				delta = DIRECTIONS[moveIdx];
			}
			path.vertices.push_back(pos);
			path.boundaryCount += length;
			pos += length * delta;
		}
		return path;
	}

	// Shoelace formula
	long long getPolygonArea(const vector<pos_t>& vertices) {
		if (vertices.size() == 0) return 0;
		long long area = 0LL;
		pos_t prevVertex = vertices.back();
		for (pos_t vertex : vertices) {
			area += prevVertex.x * vertex.y - vertex.x * prevVertex.y;
			prevVertex = vertex;
		}
		return std::abs(area) / 2;
	}

	// --------------------------- Part 1 ---------------------------------
	long long solvePart1(const vector<string>& lines) {
		auto path = createPath(lines, { 0,0 }, false);
		auto area = getPolygonArea(path.vertices);
		// Pick's theorem
		auto interior = area - path.boundaryCount / 2 + 1;
		return interior + path.boundaryCount;
	}

	// --------------------------- Part 2 ---------------------------------
	long long solvePart2(const vector<string>& lines) {
		auto path = createPath(lines, { 0,0 }, true);
		auto area = getPolygonArea(path.vertices);
		// Pick's theorem
		auto interior = area - path.boundaryCount / 2 + 1;
		return interior + path.boundaryCount;
	}
}

int AoC2023_D18::solve() {
#if 0 // tests
	auto lines = aoc::readFile("./src/D18_Lavaduct_Lagoon/small.txt");
	if (lines.empty()) return 1;
#else
	auto lines = aoc::readFile("./src/D18_Lavaduct_Lagoon/input.txt");
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
	std::cout << "Part 1 = " << part1 << "\r\n"; // 67891
	std::cout << "Part 2 = " << part2 << "\r\n"; // 94116351948493
	std::cout << "-----------------------------------------------------\r\n";
	return 0;
}
