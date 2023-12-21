#include "pch.h"
#include "Solution2.h"

namespace AoC2022_D2 {
	using namespace std;

	typedef struct round_t {
		round_t() :opponent(0), mine(0), score(0) {};
		char opponent, mine;
		int score;
	};

	vector<round_t> parseRounds(const vector<string>& lines) {
		vector<round_t> rounds;
		for (int i = 0; i < lines.size(); i++) {
			auto& line = lines[i];
			round_t round;
			round.opponent = line[0];
			round.mine = line[2];
			rounds.push_back(round);
		}
		return rounds;
	}

	int choiceToScore(char A, char B) {
		int idA = (int)(A - 'X');
		int idB = (int)(B - 'A');
		int score = 0;
		if (idA == idB) // draw
			score = idA + 1 + 3;
		else if ((idA + 2) % 3 == idB) // A wins
			score = idA + 1 + 6;
		else // B wins
			score = idA + 1;
		return score;
	}

	int resultToScore(char A, char B) {
		int idA = (int)(A - 'X');
		int idB = (int)(B - 'A');
		int score = 0;
		switch (idA)
		{
		case 0: // A loses
			// idA = idB - 1, which is the same as (idB + 2) % 3
			score += (idB + 2) % 3 + 1;
			break;
		case 1: // Draw
			// idA = idB
			score += idB + 1 + 3;
			break;
		case 2: // A wins
			// idA = (idB + 1)% 3
			score += (idB + 1) % 3 + 1 + 6;
			break;
		}
		return score;
	}

	// --------------------------- Part 1 ---------------------------------
	int solvePart1(const vector<string>& lines) {
		auto rounds = parseRounds(lines);
		int total = 0;
		for (auto& round : rounds) {
			round.score = choiceToScore(round.mine, round.opponent);
			total += round.score;
		}
		return total;
	}

	// --------------------------- Part 2 ---------------------------------
	int64_t solvePart2(const vector<string>& lines) {
		auto rounds = parseRounds(lines);
		int total = 0;
		for (auto& round : rounds) {
			round.score = resultToScore(round.mine, round.opponent);
			total += round.score;
		}
		return total;
	}
}


int AoC2022_D2::solve() {
#if 0 // test
	auto lines = aoc::readFile("./src/D2_Rock_Paper_Scissors/small.txt");
	if (lines.empty()) return 1;
#else
	auto lines = aoc::readFile("./src/D2_Rock_Paper_Scissors/input.txt");
	if (lines.empty()) return 1;
#endif
	std::ostringstream oss;
	for (auto& line : lines) {
		oss << line << "\r\n";
	}
	oss << "\r\n";
	std::cout << oss.str();

	std::cout << "-----------------------------------------------------\r\n";
	std::cout << "Part 1 = " << solvePart1(lines) << "\r\n"; // 11475
	std::cout << "Part 2 = " << solvePart2(lines) << "\r\n"; // 16862
	std::cout << "-----------------------------------------------------\r\n";

	return 0;
}