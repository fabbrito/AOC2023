#include "pch.h"
#include "Solution5.h"

namespace AoC2022_D5 {
	using namespace std;

	typedef struct move_t {
		int quant = -1, from = -1, to = -1;
	};

	typedef struct data_t {
		data_t() :nStacks(0), stacks({}), moves({}) {}
		data_t(int n) :nStacks(n), stacks(n, stack<char>()), moves({}) {}
		vector<stack<char>> stacks;
		int nStacks;
		vector<move_t> moves;
	};

	void initStacks(data_t& data, const vector<string>& lines, int pEnd) {
		for (int i = pEnd; i >= 0; i--) {
			string line = lines[i];
			for (int j = 0; j < data.nStacks; j++) {
				char elem = line[1 + 4 * j];
				if ('A' <= elem && elem <= 'Z')
					data.stacks[j].push(elem);
			}
		}
	}

	data_t parseGroups(const vector<string>& lines) {
		// find the number of stacks
		int idx = 0;
		while (idx + 1 < lines.size() && lines[idx + 1] != "") idx++;
		string line = lines[idx];
		auto info = aoc::parseString(line, " ", true);
		int nStacks = stoi(info.back());

		// populate the stacks with data
		data_t data(nStacks);
		initStacks(data, lines, idx - 1);

		// populate the moves vector with data
		for (int i = idx + 2; i < lines.size(); i++) {
			string move = lines[i];
			auto move_info = aoc::parseString(move, " ", true);
			int quant = stoi(move_info[1]);
			int from = stoi(move_info[3]) - 1;
			int to = stoi(move_info[5]) - 1;
			data.moves.push_back({ quant, from, to });
		}
		return data;
	}

	void moveOneByOne(int quant, stack<char>& fromStack, stack<char>& toStack) {
		for (int i = 0; i < quant; i++) {
			auto elem = fromStack.top();
			fromStack.pop();
			toStack.push(elem);
		}
	}

	void moveMultiple(int quant, stack<char>& fromStack, stack<char>& toStack) {
		stack<char> temp{};
		moveOneByOne(quant, fromStack, temp);
		moveOneByOne(quant, temp, toStack);
	}

	string computeMessage(data_t& data, bool multiple = false) {
		for (auto& [quant, from, to] : data.moves) {
			if (!multiple) {
				moveOneByOne(quant, data.stacks[from], data.stacks[to]);
			}
			else {
				moveMultiple(quant, data.stacks[from], data.stacks[to]);
			}

		}
		string message = "";
		for (auto& stack : data.stacks) {
			message += stack.top();
		}
		return message;
	}


	// --------------------------- Part 1 ---------------------------------
	string solvePart1(const vector<string>& lines) {
		auto data = parseGroups(lines);
		auto msg = computeMessage(data);
		return msg;
	}

	// --------------------------- Part 2 ---------------------------------
	string solvePart2(const vector<string>& lines) {
		auto data = parseGroups(lines);
		auto msg = computeMessage(data, true);
		return msg;
	}
}


int AoC2022_D5::solve() {
#if 0 // test
	auto lines = aoc::readFile("./src/D5_Supply_Stacks/small.txt");
	if (lines.empty()) return 1;
#else
	auto lines = aoc::readFile("./src/D5_Supply_Stacks/input.txt");
	if (lines.empty()) return 1;
#endif
	std::ostringstream oss;
	for (auto& line : lines) {
		oss << line << "\r\n";
	}
	oss << "\r\n";
	std::cout << oss.str();

	std::cout << "-----------------------------------------------------\r\n";
	std::cout << "Part 1 = " << solvePart1(lines) << "\r\n"; // FCVRLMVQP
	std::cout << "Part 2 = " << solvePart2(lines) << "\r\n"; // RWLWGJGFD
	std::cout << "-----------------------------------------------------\r\n";

	return 0;
}