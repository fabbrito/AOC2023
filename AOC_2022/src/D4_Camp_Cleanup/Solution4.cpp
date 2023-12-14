#include "pch.h"
#include "Solution4.h"

namespace AoC2022_D4 {
	using namespace std;

	struct range_t {
		int l, r;
	};

	struct group_t {
		group_t() :leftElf(), rightElf() {}
		range_t leftElf, rightElf;
	};

	vector<group_t> parseGroups(const vector<string>& lines) {
		vector<group_t> groups;
		for (string line : lines) {
			group_t group{};
			auto info = AoC::parseString(line, ",");
			auto leftRange = AoC::parseString(info[0], "-");
			auto rightRange = AoC::parseString(info[1], "-");
			group.leftElf = { stoi(leftRange[0]),stoi(leftRange[1]) };
			group.rightElf = { stoi(rightRange[0]),stoi(rightRange[1]) };
			groups.push_back(group);
		}
		return groups;
	}

	bool contains(const range_t& a, const range_t& b) {
		return (a.l <= b.l && b.r <= a.r) || (b.l <= a.l && a.r <= b.r);
	}

	bool overlaps(const range_t& a, const range_t& b) {
		return (a.l <= b.l && b.l <= a.r) || (a.l <= b.r && b.r <= a.r) ||
			(b.l <= a.l && a.l <= b.r) || (b.l <= a.r && a.r <= b.r);
	}

	// --------------------------- Part 1 ---------------------------------
	int solvePart1(const vector<string>& lines) {
		auto groups = parseGroups(lines);
		int count = 0;
		for (auto& group : groups) {
			if (contains(group.leftElf, group.rightElf))
				count++;
		}
		return count;
	}

	// --------------------------- Part 2 ---------------------------------
	int64_t solvePart2(const vector<string>& lines) {
		auto groups = parseGroups(lines);
		int count = 0;
		for (auto& group : groups) {
			if (overlaps(group.leftElf, group.rightElf))
				count++;
		}
		return count;
	}
}


int AoC2022_D4::solve() {
	auto lines = AoC::readFile("./src/D4_Camp_Cleanup/input.txt");
	if (lines.empty()) return 1;

	std::ostringstream oss;
	for (auto& line : lines) {
		oss << line << "\r\n";
	}
	oss << "\r\n";
	std::cout << oss.str();

	std::cout << "-----------------------------------------------------\r\n";
	std::cout << "Part 1 = " << solvePart1(lines) << "\r\n"; // 424
	std::cout << "Part 2 = " << solvePart2(lines) << "\r\n"; // 804
	std::cout << "-----------------------------------------------------\r\n";

	return 0;
}