#include "pch.h"
#include "Solution11.h"

namespace AoC2022_D11 {
	using namespace std;

	// --------------------------- Types ----------------------------------

	typedef struct monkey_t {
		int id = -1;
		int64_t inspected = 0;
		vector<int64_t> items = {};
		std::function<int64_t(int64_t, int64_t)> operation;
		int64_t constant = 1;
		int64_t divisor = 1;
		array<int, 2> nextMonkey = { 0, 0 };
	}monkey_t;

	// ------------------------- Constants --------------------------------

	static const array<string, 6> MONKEY_STRS = {
		"Monkey ",
		"  Starting items: ",
		"  Operation: new = old ",
		"  Test: divisible by ",
		"    If true: throw to monkey ",
		"    If false: throw to monkey ",
	};

	// -------------------------- Functions -------------------------------

	int64_t add(int64_t a, int64_t b) { return a + b; }
	int64_t multiply(int64_t a, int64_t b) { return a * b; }
	int64_t square(int64_t a, int64_t b) { return a * a; }

	vector<monkey_t> parseData(vector<string> lines) {
		vector<monkey_t> monkeys;
		for (int i = 0; i < lines.size(); i += 7) {
			monkey_t monkey;
			monkey.id = (int)(lines[i][MONKEY_STRS[0].length()] - '0');
			string strItems = lines[i + 1].substr(MONKEY_STRS[1].length());
			for (string item : AoC::parseString(strItems, ", ", true)) {
				monkey.items.emplace_back(std::stoll(item));
			}
			string strOperation = lines[i + 2].substr(MONKEY_STRS[2].length());
			if (strOperation[0] == '+') {
				monkey.operation = &add;
				monkey.constant = std::stoll(strOperation.substr(2));
			}
			else if (strOperation == "* old") {
				monkey.operation = &square;
			}
			else {
				monkey.operation = &multiply;
				monkey.constant = std::stoll(strOperation.substr(2));
			}
			string strDivisibleBy = lines[i + 3].substr(MONKEY_STRS[3].length());
			monkey.divisor = std::stoll(strDivisibleBy);
			string strTrue = lines[i + 4].substr(MONKEY_STRS[4].length());
			string strFalse = lines[i + 5].substr(MONKEY_STRS[5].length());
			monkey.nextMonkey = { std::stoi(strFalse) ,std::stoi(strTrue) };

			monkeys.push_back(monkey);
		}
		return monkeys;
	}

	vector<monkey_t> computeRound(vector<monkey_t> state, int64_t LCM, int64_t relief = 3) {
		for (int turn = 0; turn < state.size(); turn++) {
			auto& monkey = state[turn];
			vector<int64_t> items = monkey.items;
			monkey.items.clear();
			for (auto& item : items) {
				monkey.inspected++;
				auto worryLevel = monkey.operation(item, monkey.constant);
				worryLevel /= relief;
				worryLevel %= LCM; // uses the LCM of all divisors
				bool isDivisible = worryLevel % monkey.divisor == 0;
				auto nextId = monkey.nextMonkey[(int)(isDivisible)];

				state[nextId].items.push_back(worryLevel);
			}
		}
		return state;
	}

	// --------------------------- Part 1 ---------------------------------
	int64_t solvePart1(const vector<string>& lines) {
		auto monkeyBusiness = parseData(lines);
		int64_t LCM = 1;
		for (auto& monkey : monkeyBusiness)
			LCM = std::lcm(monkey.divisor, LCM);
		int rounds = 20;
		for (int i = 0; i < rounds; i++) {
			monkeyBusiness = computeRound(monkeyBusiness, LCM, 3);
		}
		std::sort(monkeyBusiness.begin(), monkeyBusiness.end(), [](const monkey_t& a, const monkey_t& b) {return a.inspected > b.inspected; });
		return monkeyBusiness[0].inspected * monkeyBusiness[1].inspected;
	}

	// --------------------------- Part 2 ---------------------------------
	int64_t solvePart2(const vector<string>& lines) {
		auto monkeyBusiness = parseData(lines);
		int64_t LCM = 1;
		for (auto& monkey : monkeyBusiness)
			LCM = std::lcm(monkey.divisor, LCM);
		int rounds = 10000;
		for (int i = 1; i <= rounds; i++) {
			monkeyBusiness = computeRound(monkeyBusiness, LCM, 1);
			/*if (i == 1 || i % (rounds / 10) == 0) {
				std::cout << "Round " << i << (i == rounds ? " (last)" : "") << std::endl;
				for (auto& monkey : monkeyBusiness) {
					std::cout << monkey.id << " " << monkey.inspected << std::endl;
				}
			}*/
		}
		std::sort(monkeyBusiness.begin(), monkeyBusiness.end(), [](const monkey_t& a, const monkey_t& b) {return a.inspected > b.inspected; });
		return monkeyBusiness[0].inspected * monkeyBusiness[1].inspected;
	}
}

int AoC2022_D11::solve() {
#if 1 // smaller test
	auto lines = AoC::readFile("./src/D11_Monkey_in_the_Middle/small.txt");
	if (lines.empty()) return 1;
#else
	auto lines = AoC::readFile("./src/D11_Monkey_in_the_Middle/input.txt");
	if (lines.empty()) return 1;
#endif
	// pretty print
	std::ostringstream oss;
	for (int i = 0; i < lines.size(); i++) {
		oss << i;
		if (i <= 9) oss << string(3, ' ');
		else if (10 <= i && i <= 99) oss << string(2, ' ');
		else if (100 <= i && i <= 999) oss << " ";
		oss << "|" << lines[i] << "\r\n";
	}
	oss << "\r\n";
	cout << oss.str();

	auto part1 = solvePart1(lines);
	auto part2 = solvePart2(lines);

	std::cout << "-----------------------------------------------------\r\n";
	std::cout << "Part 1 = " << part1 << "\r\n"; // 62491
	std::cout << "Part 2 = " << part2 << "\r\n"; // 17408399184
	std::cout << "-----------------------------------------------------\r\n";
	return 0;
}
