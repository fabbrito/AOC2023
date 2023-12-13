#include "pch.h"
#include "Solution4.h"

namespace AoC_D4 {
	using namespace std;
	class Solution;
	class Part1;
	class Part2;
}

class AoC_D4::Solution {
public:
	struct CardData {
		int id = -1, wins = 0, points = 0;
		vector<int> winning_numbers, card_numbers;
	};
protected:
	static int getCardId(string& str) {
		auto card_info = AoC::parseString(str, ": ");
		AoC::parseString(card_info.front(), " ");
		return stoi(card_info.front());
	}

	static vector<int> getNumbers(string str) {
		vector<int> output;
		auto str_vec = AoC::parseString(str, " ");
		for (auto& elem : str_vec) {
			// to ignore double spaces
			if (elem.size() == 0) continue;
			output.push_back(stoi(elem));
		}
		return output;
	}

	static CardData getCardData(string game) {
		CardData output;
		output.id = getCardId(game);

		auto card_info = AoC::parseString(game, " | ");
		output.winning_numbers = getNumbers(card_info.front());
		output.card_numbers = getNumbers(card_info.back());

		// sort and bin search for matches
		sort(output.winning_numbers.begin(), output.winning_numbers.end());
		sort(output.card_numbers.begin(), output.card_numbers.end());
		for (const auto& number : output.card_numbers) {
			if (std::binary_search(output.winning_numbers.begin(), output.winning_numbers.end(), number)) {
				output.wins++;
			}
		}

		output.points = static_cast<int>(pow(2, output.wins - 1));
		return output;
	}
};

class AoC_D4::Part1 : public AoC_D4::Solution {
public:
	int sumPoints(vector<string> game_lines) {
		int sum = 0;
		for (string& line : game_lines) {
			auto card_data = getCardData(line);
			sum += card_data.points;
		}
		return sum;
	}
};

class AoC_D4::Part2 : public AoC_D4::Solution {
public:
	int totalOfCards(vector<string> game_lines) {
		int n = game_lines.size();
		vector<int> cards(n, 1);
		int total = 0;
		for (int i = 0; i < n; i++) {
			auto data = getCardData(game_lines[i]);
			for (int j = 1; j < n && j <= data.wins; j++) {
				cards[i + j] += cards[i];
			}
			total += cards[i];
		}
		return total;
	}
};

int AoC_D4::solve()
{
	auto lines = AoC::readFile("./src/D4_Scratch_Cards/input.txt");
	if (lines.empty()) return 1;

	std::ostringstream oss;
	for (auto& line : lines) {
		oss << line << "\r\n";
	}
	oss << "\r\n";
	cout << oss.str();

	Part1 sol1;
	auto sum_points = sol1.sumPoints(lines);
	std::cout << "-----------------------------------------------------\r\n";
	std::cout << "Sum of points = " << sum_points << "\r\n"; // 21138
	std::cout << "-----------------------------------------------------\r\n";

	Part2 sol2;
	auto total_cards = sol2.totalOfCards(lines);
	std::cout << "-----------------------------------------------------\r\n";
	std::cout << "Total scratchcards = " << total_cards << "\r\n"; // 7185540
	std::cout << "-----------------------------------------------------\r\n";


	return 0;
}
