#include "pch.h"
#include "Solution11.h"

namespace AoC_D11 {
	using namespace std;
	class Solution;
};

class AoC_D11::Solution {
private:
	struct pos_t {
		int r = -1, c = -1;
		bool operator==(const pos_t& rhs) { return r == rhs.r && c == rhs.c; }
		bool operator!=(const pos_t& rhs) { return !(*this == rhs); }
		pos_t& operator+=(const pos_t& rhs) { r += rhs.r; c += rhs.c; return *this; }
		friend pos_t operator+(pos_t lhs, const pos_t& rhs) { lhs += rhs; return lhs; }
		friend std::ostream& operator<<(std::ostream& out, const pos_t& pos) {
			return out << "{" << pos.r << ", " << pos.c << "}";
		}
	};

	struct map_t {
		char at(const pos_t& p) const { return grid[p.c * width + p.r]; }
		char& at(const pos_t& p) { return grid[p.c * width + p.r]; }
		char at(const int& r, const int& c) const { return grid[c * width + r]; }
		char& at(const int& r, const int& c) { return grid[c * width + r]; }
		std::vector<char> grid;
		int width = 0;
		int height = 0;
	};

	struct space_t {
		map_t image, map;
		vector<pos_t> stars;
	};

	const pos_t up{ 0, -1 };
	const pos_t down{ 0, 1 };
	const pos_t left{ -1, 0 };
	const pos_t right{ 1, 0 };
public:
	Solution(const vector<string>& lines) {
		mSpace.image = getImage(lines);
		mSpace.map = expandImage(mSpace.image);
	}

	int64_t solvePart1() {
		return 0;
	}

	int64_t solvePart2() {
		return 0;
	}
private:
	space_t mSpace;
private:
	map_t getImage(const vector<string>& lines) {
		map_t image;
		image.height = (int)lines.size();
		image.width = (int)lines.front().size();
		for (auto& line : lines) {
			std::copy(line.begin(), line.end(), std::back_inserter(image.grid));
		}
		return image;
	}

	map_t expandImage(const map_t& image, const char& c = '.') {
		map_t map;
		auto& imageGrid = image.grid;
		int ih = image.height, iw = image.width;

		for (int i = 0; i < image.height; i++) {
			bool empty = true;
			for (int j = 0; j < image.width; j++) {
				if (image.at(i,j) != c) {
					empty = false;
					break;
				}
			}
			cout << i << " "<< i+image.width << " " << empty << endl;
		}
		return map;
	}

	vector<pos_t> findStars(const map_t& map) {
		return {};
	}

};

int AoC_D11::solve() {
	auto lines = AoC::readFile("./src/D11_Cosmic_Expansion/small.txt");
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
