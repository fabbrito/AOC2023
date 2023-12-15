#include "pch.h"
#include "Solution15.h"

namespace AoC2023_D15 {
	using namespace std;

	// --------------------------- Types ----------------------------------
	
	struct box_t {
		typedef struct item_t {
			string name;
			int lens;
		};

		int id;
		vector<item_t> items;

		box_t() :id(-1), items({}) {}
		box_t(int i) :id(i), items({}) {}

		void addItem(const string& name, const int& lens) {
			auto it = std::find_if(items.begin(), items.end(), [&](const item_t& a) {return a.name == name; });
			if (it != items.end()) {
				it->lens = lens;
			}
			else {
				items.push_back({ name, lens });
			}
		}

		void removeItem(const string& name) {
			auto it = std::find_if(items.begin(), items.end(), [&](const item_t& a) {return a.name == name; });
			if (it != items.end())
				items.erase(it);
		}

		int64_t getFocusingPower() {
			int64_t power = 0;
			for (int i = 0; i < items.size(); i++) {
				power += (id + 1) * (i + 1) * items[i].lens;
			}
			return power;
		}
	};

	// ------------------------- Constants --------------------------------

	// -------------------------- Functions -------------------------------

	int hasher(string str) {
		int hash = 0;
		for (auto& elem : str) {
			hash = ((hash + (int)elem) * 17) % 256;
		}
		return hash;
	}

	void parseCommand(string info, string& name, char& command, int& lens) {
		auto itEqual = info.find("=");
		auto itMinus = info.find("-");
		if (itEqual != std::string::npos) {
			name = info.substr(0, itEqual), command = info[itEqual], lens = stoi(info.substr(itEqual + 1));
		}
		else if (itMinus != std::string::npos) {
			name = info.substr(0, itMinus), command = info[itMinus];
		}
	}

	void computeCommand(box_t& box, const string& name, const char& command, const int& lens) {
		if (command == '=')
			box.addItem(name, lens);
		else if (command == '-')
			box.removeItem(name);
	}

	vector<box_t> parseSequence(const vector<string>& lines) {
		string line = lines.front();
		auto data = AoC::parseString(line, ",");
		vector<box_t> boxes;
		for (auto& info : data) {
			string name = "";
			char command = NULL;
			int lens = 0;
			parseCommand(info, name, command, lens);
			int boxId = hasher(name);

			auto it = std::find_if(boxes.begin(), boxes.end(), [&](const box_t& a) {return a.id == boxId; });
			if (it != boxes.end()) {
				size_t idx = std::distance(boxes.begin(), it);
				auto& box = boxes[idx];
				computeCommand(box, name, command, lens);
			}
			else {
				box_t box(boxId);
				computeCommand(box, name, command, lens);
				boxes.push_back(box);
			}
		}
		return boxes;
	}

	// --------------------------- Part 1 ---------------------------------
	int64_t solvePart1(const vector<string>& lines) {
		string line = lines.front();
		auto data = AoC::parseString(line, ",");
		int64_t sum = 0;
		for (auto& str : data) {
			sum += (int64_t)hasher(str);
		}
		return sum;
	}

	// --------------------------- Part 2 ---------------------------------
	int64_t solvePart2(const vector<string>& lines) {
		auto boxes = parseSequence(lines);
		int64_t power = 0;
		for (auto& box : boxes) {
			power += box.getFocusingPower();
		}
		return power;
	}
}

int AoC2023_D15::solve() {
#if 0 // tests
	assert(hasher("HASH") == 52);
	auto lines = AoC::readFile("./src/D15_Lens_Library/small.txt");
	if (lines.empty()) return 1;
#else
	auto lines = AoC::readFile("./src/D15_Lens_Library/input.txt");
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
	std::cout << "Part 1 = " << part1 << "\r\n"; // 510792
	std::cout << "Part 2 = " << part2 << "\r\n"; // 269410
	std::cout << "-----------------------------------------------------\r\n";
	return 0;
}
