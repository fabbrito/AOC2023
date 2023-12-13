#include "pch.h"
#include "Solution13.h"

namespace AoC_D13 {
	using namespace std;

	typedef struct field_t {
		field_t() :data({}), height(0), width(0) {}
		vector<char> data;
		int height, width;
		char at(const int& row, const int& col) const { return data[row * width + col]; }
		char& at(const int& row, const int& col) { return data[row * width + col]; }
		void clear() { data = {}, height = 0, width = 0; }
	};

	vector<field_t> parseFields(const vector<string>& lines) {
		vector<field_t> fields;
		field_t field;
		for (int i = 0; i < lines.size(); i++) {
			auto& line = lines[i];
			if (line == "" || i == lines.size() - 1) {
				fields.push_back(field);
				field.clear();
			}
			else {
				std::copy(line.begin(), line.end(), std::back_inserter(field.data));
				field.width = line.length();
				field.height++;
			}

		}
		return fields;
	}

	// vertical line in check is to the right of column 'col'
	bool validVerticalLine(const field_t& field, int col, int allowedMistakes) {
		if (col >= field.width - 1) return false;
		int mistakes = 0;
		for (int j = 0; j <= col; j++) {
			if (col + j >= field.width - 1) continue;
			for (int row = 0; row < field.height; row++) {
				if (field.at(row, col - j) != field.at(row, col + j + 1)) {
					if (mistakes >= allowedMistakes)
						return false;
					mistakes++;
				}
			}
		}
		return mistakes == allowedMistakes;
	}
	// horizontal line in check is to the bottom of row 'row'
	bool validHorizontalLine(const field_t& field, int row, int allowedMistakes) {
		if (row >= field.height - 1) return false;
		int mistakes = 0;
		for (int i = 0; i <= row; i++) {
			if (row + i >= field.height - 1) continue;
			for (int col = 0; col < field.width; col++) {
				if (field.at(row - i, col) != field.at(row + i + 1, col)) {
					if (mistakes >= allowedMistakes)
						return false;
					mistakes++;
				}
			}
		}
		return mistakes == allowedMistakes;
	}

	int getVerticalLine(const field_t& field, int allowedMistakes) {
		for (int lineCol = 0; lineCol < field.width - 1; lineCol++) {
			if (validVerticalLine(field, lineCol, allowedMistakes)) {
				return lineCol;
			}
		}
		return -1;
	}

	int getHorizontalLine(const field_t& field, int allowedMistakes) {
		for (int lineRow = 0; lineRow < field.height - 1; lineRow++) {
			if (validHorizontalLine(field, lineRow, allowedMistakes)) {
				return lineRow;
			}
		}
		return -1;
	}

	int64_t countBeforeMirrorLine(const field_t& field, int allowedMistakes = 0) {
		int vLine = getVerticalLine(field, allowedMistakes);
		if (vLine != -1) return (int64_t)vLine + 1;
		int hLine = getHorizontalLine(field, allowedMistakes);
		if (hLine != -1) return ((int64_t)hLine + 1) * 100;
		__debugbreak();
		return 0;
	}

	// --------------------------- Part 1 ---------------------------------
	int64_t solvePart1(const vector<string>& lines) {
		auto fields = parseFields(lines);
		int64_t sum = 0;
		for (auto& field : fields) {
			sum += countBeforeMirrorLine(field);
		}
		return sum;
	}

	// --------------------------- Part 2 ---------------------------------
	int64_t solvePart2(const vector<string>& lines) {
		auto fields = parseFields(lines);
		int64_t sum = 0;
		for (auto& field : fields) {
			sum += countBeforeMirrorLine(field, 1);
		}
		return sum;
	}
}

int AoC_D13::solve() {
	auto lines = AoC::readFile("./src/D13_Point_of_Incidence/input.txt");
	if (lines.empty()) return 1;

	std::ostringstream oss;
	for (auto& line : lines) {
		oss << line << "\r\n";
	}
	oss << "\r\n";
	cout << oss.str();

	std::cout << "-----------------------------------------------------\r\n";
	std::cout << "Part 1 = " << solvePart1(lines) << "\r\n"; // 33356
	std::cout << "Part 2 = " << solvePart2(lines) << "\r\n"; // 28475
	std::cout << "-----------------------------------------------------\r\n";

	return 0;
}
