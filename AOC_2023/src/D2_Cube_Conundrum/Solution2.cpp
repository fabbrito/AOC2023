#include "pch.h"
#include "Solution2.h"

int AoC_D2::solve()
{
	auto lines = AoC::readFile("./src/D2_Cube_Conundrum/input.txt");
	if (lines.empty()) return 1;

	std::unordered_map<std::string, int> config = { 
		{"red",12},
		{"green",13},
		{"blue",14},
	};

	int sum_ids = 0, sum_powers = 0;
	for (auto& line : lines) {
		//std::cout << "-----------------------------------------------------\r\n";
		//std::cout << line << std::endl;
		auto game = AoC::parseString(line, ": ");

		AoC::parseString(game[0], " ");
		int game_id = std::atoi(game[0].c_str());
		//std::cout << game_id << std::endl;

		auto rounds = AoC::parseString(game[1], "; ");

		std::unordered_map<std::string, int> cube_map;

		for (auto& round : rounds) {
			auto tokens = AoC::parseString(round, ", ");
			for (auto& token : tokens) {
				auto cubes = AoC::parseString(token, " ");
				cube_map[token] = std::max(cube_map[token], std::atoi(cubes[0].c_str()));
			}
		}

		int power = 1;
		bool possible_game = true;
		for (auto& [color, quant] : cube_map) {
			//std::cout << "max " << color << " -> " << quant << "\r\n";
			power *= quant;
			if (config[color] < quant) {
				possible_game = false;
			}
		}
		sum_powers += power;
		if (possible_game) {
			sum_ids += game_id;
		}
	}

	std::cout << "-----------------------------------------------------\r\n";
	std::cout << "Possible games: Sum of the ids = " << sum_ids << "\r\n";
	std::cout << "-----------------------------------------------------\r\n";
	std::cout << "Sum of powers = " << sum_powers << "\r\n";
	std::cout << "-----------------------------------------------------\r\n";

	return 0;
}
