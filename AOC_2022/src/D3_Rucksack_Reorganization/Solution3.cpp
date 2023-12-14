#include "pch.h"
#include "Solution3.h"

namespace AoC2022_D3 {
	using namespace std;

	typedef struct rucksack_t {
		rucksack_t() :items({}), id({ 0 }), priority(0) {};
		vector<char> items;
		bitset<52> id;
		int priority;
	};

	typedef struct group_t {
		group_t() :rucksacks({}), priority(0) {};
		array<rucksack_t, 3> rucksacks;
		int priority;
	};

	void computeId(rucksack_t& rucksack) {
		auto& items = rucksack.items;
		for (auto& item : items) {
			int idx = 0;
			if ('a' <= item && item <= 'z')
				idx = (int)(item - 'a');
			else if ('A' <= item && item <= 'Z')
				idx = (int)(item - 'A') + 26;
			rucksack.id.set(idx);
		}
	}

	void computePriority(rucksack_t& rucksack) {
		auto& items = rucksack.items;
		int n = items.size();
		for (int i = 0; i < n / 2; i++) {
			for (int j = n / 2; j < n; j++) {
				if (items[i] == items[j]) {
					if ('a' <= items[i] && items[i] <= 'z')
						rucksack.priority = (int)(items[i] - 'a') + 1;
					else if ('A' <= items[i] && items[i] <= 'Z')
						rucksack.priority = (int)(items[i] - 'A') + 26 + 1;
					return;
				}
			}
		}
	}

	void computeGroupPriority(group_t& group) {
		constexpr uint64_t UINT52_MAX = (1Ui64 << 53Ui64) - 1Ui64;
		bitset<52> combined(UINT52_MAX);
		for (auto& rucksack : group.rucksacks) {
			combined &= rucksack.id;
		}
		uint64_t number = combined.to_ullong();
		group.priority = 0;
		while (number > 0) {
			number >>= 1;
			group.priority++;
		}
	}

	vector<rucksack_t> parseRucksacks(const vector<string>& lines) {
		vector<rucksack_t> collection;
		for (int i = 0; i < lines.size(); i++) {
			auto& line = lines[i];
			rucksack_t rucksack;
			std::copy(line.begin(), line.end(), std::back_inserter(rucksack.items));
			computePriority(rucksack);
			collection.push_back(rucksack);
		}
		return collection;
	}

	vector<group_t> parseGroups(const vector<string>& lines, int groupSize) {
		vector<group_t> groups;
		for (int i = 0; i <= lines.size() - groupSize; i += groupSize) {
			group_t group;
			for (int j = 0; j < groupSize; j++) {
				auto& line = lines[i + j];
				auto& rucksack = group.rucksacks[j];
				std::copy(line.begin(), line.end(), std::back_inserter(rucksack.items));
				computeId(rucksack);
			}
			computeGroupPriority(group);
			groups.push_back(group);
		}
		return groups;
	}

	// --------------------------- Part 1 ---------------------------------
	int solvePart1(const vector<string>& lines) {
		auto collection = parseRucksacks(lines);
		int sum = 0;
		for (auto& rucksack : collection) {
			sum += rucksack.priority;
		}
		return sum;
	}

	// --------------------------- Part 2 ---------------------------------
	int64_t solvePart2(const vector<string>& lines) {
		auto groups = parseGroups(lines, 3);
		int sum = 0;
		for (auto& group : groups) {
			sum += group.priority;
		}
		return sum;
	}
}


int AoC2022_D3::solve() {
	auto lines = AoC::readFile("./src/D3_Rucksack_Reorganization/input.txt");
	if (lines.empty()) return 1;

	std::ostringstream oss;
	for (auto& line : lines) {
		oss << line << "\r\n";
	}
	oss << "\r\n";
	std::cout << oss.str();

	std::cout << "-----------------------------------------------------\r\n";
	std::cout << "Part 1 = " << solvePart1(lines) << "\r\n"; // 7674
	std::cout << "Part 2 = " << solvePart2(lines) << "\r\n"; // 2805
	std::cout << "-----------------------------------------------------\r\n";

	return 0;
}