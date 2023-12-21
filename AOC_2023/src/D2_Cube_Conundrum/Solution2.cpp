#include "pch.h"
#include "Solution2.h"

namespace AoC2023_D2 {
	using namespace std;

	// --------------------------- Types ----------------------------------

	struct game_t {
		int id = 0;
		std::unordered_map<std::string, int> tokenMap;
		int rounds = 0;
	};

	// -------------------------- Functions -------------------------------
	vector<game_t> parseGames(const vector<string>& lines) {
		vector<game_t> games;
		for (string line : lines) {
			game_t game;
			auto infoVec = aoc::parseString(line, ": ", true);
			game.id = std::stoi(aoc::parseString(infoVec.front(), " ").back());

			auto roundsVec = aoc::parseString(infoVec.back(), "; ", true);
			game.rounds = roundsVec.size();
			for (string round : roundsVec) {
				auto turnsVec = aoc::parseString(round, ", ");
				for (auto& turn : turnsVec) {
					auto tokenVec = aoc::parseString(turn, " ");
					auto& token = tokenVec[1];
					auto numCubes = std::stoi(tokenVec[0]);
					if (game.tokenMap.find(token) != game.tokenMap.end()) {
						game.tokenMap[token] = std::max(game.tokenMap[token], numCubes);
					}
					else {
						game.tokenMap[token] = numCubes;
					}
				}
			}
			games.push_back(game);
		}
		return games;
	}


	// --------------------------- Part 1 ---------------------------------
	int solvePart1(const vector<string>& lines) {
		auto games = parseGames(lines);
		std::unordered_map<std::string, int> gameConfig = { {"red",12},{"green",13},{"blue",14} };
		int sumIds = 0;
		for (auto& game : games) {
			bool possibleGame = true;
			for (auto& [token, numCubes] : game.tokenMap) {
				if (gameConfig[token] < numCubes) {
					possibleGame = false;
					break;
				}
			}
			if (possibleGame) {
				sumIds += game.id;
			}
		}
		return sumIds;
	}

	// --------------------------- Part 2 ---------------------------------
	int solvePart2(const vector<string>& lines) {
		auto games = parseGames(lines);
		int sumPowers = 0;
		for (auto& game : games) {
			int power = 1;
			for (auto& [_, numCubes] : game.tokenMap) {
				power *= numCubes;
			}
			sumPowers += power;
		}
		return sumPowers;
	}
}

int AoC2023_D2::solve()
{
#if 1 // tests
	auto lines = aoc::readFile("./src/D2_Cube_Conundrum/small.txt");
	if (lines.empty()) return 1;
#else
	auto lines = aoc::readFile("./src/D2_Cube_Conundrum/input.txt");
	if (lines.empty()) return 1;
#endif
	std::ostringstream oss;
	for (auto& line : lines) {
		oss << line << "\r\n";
	}
	oss << "\r\n";
	std::cout << oss.str();
	auto part1 = solvePart1(lines);
	auto part2 = solvePart2(lines);

	std::cout << "-----------------------------------------------------\r\n";
	std::cout << "Part 1 = " << part1 << "\r\n"; // 2528
	std::cout << "Part 2 = " << part2 << "\r\n"; // 67363
	std::cout << "-----------------------------------------------------\r\n";
	return 0;
}