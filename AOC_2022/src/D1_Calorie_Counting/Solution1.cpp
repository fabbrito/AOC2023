#include "pch.h"
#include "Solution1.h"

namespace AoC2022_D1 {
	using namespace std;

	typedef struct bag_t {
		bag_t() :foods({}), totalCalories(0) {};
		vector<int64_t> foods;
		int64_t totalCalories;
		void clear() { foods = {}, totalCalories = 0; }
	};

	vector<bag_t> parseBags(const vector<string>& lines) {
		vector<bag_t> bags;
		bag_t bag;
		for (int i = 0; i < lines.size(); i++) {
			auto& line = lines[i];
			if (line == "") {
				bags.push_back(bag);
				bag.clear();
			}
			else {
				auto calories = stoll(line);
				bag.foods.emplace_back(calories);
				bag.totalCalories += calories;
			}
		}
		bags.push_back(bag);
		std::sort(bags.begin(), bags.end(), [](const bag_t& a, const bag_t& b) {return a.totalCalories < b.totalCalories; });
		return bags;
	}

	// --------------------------- Part 1 ---------------------------------
	int64_t solvePart1(const vector<string>& lines) {
		auto bags = parseBags(lines);
		return bags[bags.size() - 1].totalCalories;
	}

	// --------------------------- Part 2 ---------------------------------
	int64_t solvePart2(const vector<string>& lines) {
		auto bags = parseBags(lines);
		int64_t sum = 0;
		for (int i = 0; i < 3; i++) {
			sum += bags[bags.size() - 1 - i].totalCalories;
		}
		return sum;
	}
}


int AoC2022_D1::solve() {
#if 0 // test
	auto lines = aoc::readFile("./src/D1_Calorie_Counting/small.txt");
	if (lines.empty()) return 1;
#else
	auto lines = aoc::readFile("./src/D1_Calorie_Counting/input.txt");
	if (lines.empty()) return 1;
#endif

	std::ostringstream oss;
	for (auto& line : lines) {
		oss << line << "\r\n";
	}
	oss << "\r\n";
	std::cout << oss.str();

	std::cout << "-----------------------------------------------------\r\n";
	std::cout << "Part 1 = " << solvePart1(lines) << "\r\n"; // 69528
	std::cout << "Part 2 = " << solvePart2(lines) << "\r\n"; // 206152
	std::cout << "-----------------------------------------------------\r\n";

	return 0;
}