#include "pch.h"
#include "Solution11.h"

namespace AoC2023_D11 {
	using namespace std;
	class Solution;
};

class AoC2023_D11::Solution {
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

	const char emptySpace = '.';
	const char star = '#';

private:
	map_t mMap;

public:
	Solution(const vector<string>& lines) {
		mMap = getImage(lines);
	}

	int64_t solvePart1() {
		vector<pos_t> stars = findStars(mMap, 2);
		return sumAllDistances(stars);
	}

	int64_t solvePart2() {
		vector<pos_t> stars = findStars(mMap, 1000000);
		return sumAllDistances(stars);
	}

	void printOriginalMap() {
		cout << mMap;
	}
private:
	int64_t sumAllDistances(const vector<pos_t>& stars) {
		int numStars = stars.size();
		//vector<vector<int>> distances(numStars, vector<int>(numStars, 0));
		int64_t sum = 0;
		for (int i = 0; i < numStars; i++) {
			for (int j = 0; j < i; j++) {
				//distances[i][j] = distance(stars[i], stars[j]);
				sum += (int64_t)distance(stars[i], stars[j]);
			}
		}
		return sum;
	}

	int distance(const pos_t& a, const pos_t& b) {
		return abs(a.r - b.r) + abs(a.c - b.c);
	}

	map_t getImage(const vector<string>& lines) {
		map_t image;
		image.height = (int)lines.size();
		image.width = (int)lines.front().size();
		for (auto& line : lines) {
			std::copy(line.begin(), line.end(), std::back_inserter(image.grid));
		}
		return image;
	}

	vector<pos_t> findStars(const map_t& map, int expanseFactor) {
		vector<pos_t> stars;

		auto& height = map.height;
		auto& width = map.width;

		vector<int> rowOffsets(height, 0);
		vector<int> colOffsets(width, 0);

		for (int i = 0; i < map.height; i++) {
			bool emptyRow = true;
			for (int j = 0; j < map.width; j++) {
				if (map.at(i, j) == star) {
					emptyRow = false;
				}
			}
			if (i == 0) rowOffsets[i] = emptyRow ? 1 : 0;
			else rowOffsets[i] = emptyRow ? rowOffsets[i - 1] + 1 : rowOffsets[i - 1];
		}

		for (int j = 0; j < map.width; j++) {
			bool emptyCol = true;
			for (int i = 0; i < map.height; i++) {
				if (map.at(i, j) == star) {
					emptyCol = false;
				}
			}
			if (j == 0) colOffsets[j] = emptyCol ? 1 : 0;
			else colOffsets[j] = emptyCol ? colOffsets[j - 1] + 1 : colOffsets[j - 1];
		}

		for (int i = 0; i < map.height; i++) {
			for (int j = 0; j < map.width; j++) {
				if (map.at(i, j) == star) {
					int row = i + rowOffsets[i] * (expanseFactor - 1);
					int col = j + colOffsets[j] * (expanseFactor - 1);
					stars.push_back({ row,col });
				}
			}
		}
		return stars;
	}
};

int AoC2023_D11::solve() {
	auto lines = aoc::readFile("./src/D11_Cosmic_Expansion/input.txt");
	if (lines.empty()) return 1;

	std::ostringstream oss;
	for (auto& line : lines) {
		oss << line << "\r\n";
	}
	oss << "\r\n";
	cout << oss.str();

	Solution solution(lines);
	solution.printOriginalMap();
	std::cout << "-----------------------------------------------------\r\n";
	std::cout << "Part 1 = " << solution.solvePart1() << "\r\n"; // 6725
	std::cout << "Part 2 = " << solution.solvePart2() << "\r\n"; // 357134560737
	std::cout << "-----------------------------------------------------\r\n";

	return 0;
}
