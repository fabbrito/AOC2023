#include "pch.h"
#include "Solution10.h"

namespace AoC2023_D10 {
	using namespace std;
	class Solution;
};

class AoC2023_D10::Solution {
private:
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

	typedef struct map_t {
		char at(const pos_t& p) const { return grid[p.r * width + p.c]; }
		char& at(const pos_t& p) { return grid[p.r * width + p.c]; }
		char at(const int& r, const int& c) const { return grid[r * width + c]; }
		char& at(const int& r, const int& c) { return grid[r * width + c]; }
		std::vector<char> grid;
		int width = 0;
		int height = 0;

		friend std::ostream& operator<<(std::ostream& out, const map_t& map) {
			std::stringstream ss;
			ss << "\r\n";
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

	struct loop_t {
		map_t map;
		vector<pos_t> verts;
		int steps = 0;

	};
public:
	Solution(const vector<string>& lines) {
		setMap(lines);
		m_start_pos = findStart();
		m_loop = findLoop();
	}

	int64_t solvePart1() {
		auto steps = m_loop.steps;
		int64_t dist = steps / 2;
		return steps % 2 == 0 ? dist : dist + 1;
	}

	int64_t solvePart2() {
		int square = squareOfPolynomial(m_loop.verts);
		int perimeter = m_loop.steps;
		return square + 1 - m_loop.steps / 2;
	}

	void printMap() {
		cout << m_map;
	}

	void printLoop() {
		cout << m_loop.map;
	}

private:
	void setMap(const vector<string>& lines) {
		m_map.height = 0;
		for (auto& line : lines) {
			std::copy(line.begin(), line.end(), std::back_inserter(m_map.grid));
			m_map.width = (int)line.size();
			m_map.height++;
		}
	}

	pos_t findStart(const char& id = 'S') {
		auto start_it = std::find(m_map.grid.begin(), m_map.grid.end(), id);
		auto c_start = (int)std::distance(m_map.grid.begin(), start_it) % m_map.width;
		auto r_start = (int)std::distance(m_map.grid.begin(), start_it) / m_map.width;
		return { r_start, c_start };
	}

	loop_t findLoop() {
		loop_t loop;
		loop.map = m_map;
		int h = m_map.height, w = m_map.width;
		std::fill(loop.map.grid.begin(), loop.map.grid.end(), '.');
		loop.map.at(m_start_pos) = '*';

		pos_t dir;
		pos_t pos = m_start_pos;

		if ((pos + down).inRange(h, w) && (m_map.at(pos + down) == '|' || m_map.at(pos + down) == 'L' || m_map.at(pos + down) == 'J')) {
			dir = down;
		}
		else if ((pos + up).inRange(h, w) && (m_map.at(pos + up) == '|' || m_map.at(pos + up) == 'F' || m_map.at(pos + up) == '7')) {
			dir = up;
		}
		else if ((pos + left).inRange(h, w) && (m_map.at(pos + left) == '-' || m_map.at(pos + left) == 'F' || m_map.at(pos + left) == 'L')) {
			dir = left;
		}
		else if ((pos + right).inRange(h, w) && (m_map.at(pos + right) == '-' || m_map.at(pos + right) == '7' || m_map.at(pos + right) == 'J')) {
			dir = right;
		}

		loop.verts.push_back(pos);
		pos += dir;
		loop.steps++;

		while (pos != m_start_pos) {
			char pipe = m_map.at(pos);
			if (pipe == 'L') {
				dir = dir == down ? right : up;
				loop.verts.push_back(pos);
			}
			else if (pipe == 'J') {
				dir = dir == down ? left : up;
				loop.verts.push_back(pos);
			}
			else if (pipe == '7') {
				dir = dir == up ? left : down;
				loop.verts.push_back(pos);
			}
			else if (pipe == 'F') {
				dir = dir == up ? right : down;
				loop.verts.push_back(pos);
			}
			loop.map.at(pos) = '*';
			pos += dir;
			loop.steps++;
		};

		return loop;
	}

	int squareOfPolynomial(vector<pos_t>& points) {
		if (points.size() == 0) return 0;
		int square = 0;
		int num_points = points.size();
		for (int i = 1; i < num_points; i++) {
			square += points[i - 1].r * points[i].c - points[i].r * points[i - 1].c;
		}
		square += points[num_points - 1].r * points[0].c - points[0].r * points[num_points - 1].c;
		return abs(square) / 2;
	}

private:
	const pos_t up{ -1, 0 };
	const pos_t down{ 1, 0 };
	const pos_t left{ 0, -1 };
	const pos_t right{ 0, 1 };

	map_t m_map;
	loop_t m_loop;
	pos_t m_start_pos;
};


int AoC2023_D10::solve() {
	auto lines = aoc::readFile("./src/D10_Pipe_Maze/small.txt");
	if (lines.empty()) return 1;

	std::ostringstream oss;
	for (auto& line : lines) {
		oss << line << "\r\n";
	}
	oss << "\r\n";
	cout << oss.str();

	Solution solution(lines);
	std::cout << "-----------------------------------------------------\r\n";
	std::cout << "Part 1 = " << solution.solvePart1() << "\r\n"; // 6725
	std::cout << "Part 2 = " << solution.solvePart2() << "\r\n"; // 383
	std::cout << "-----------------------------------------------------\r\n";

	return 0;
}
