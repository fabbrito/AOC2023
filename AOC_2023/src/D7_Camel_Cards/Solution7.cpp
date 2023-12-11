#include "pch.h"
#include "Solution7.h"

namespace AoC_D7 {
	using namespace std;
	class Solution;
}

class AoC_D7::Solution {
public:
	uint64_t solvePart1(const vector<string>& lines) {
		const string order = "23456789TJQKA";
		auto games = computeGames(lines, order);
		uint64_t total_winnings = 0;
		for (size_t i = 0; i < games.size(); i++) {
			total_winnings += (i + 1) * games[i].bid;
		}
		return total_winnings;
	}

	uint64_t solvePart2(const vector<string>& lines) {
		const string order = "J23456789TQKA";
		auto games = computeGames(lines, order, 0);
		uint64_t total_winnings = 0;
		for (size_t i = 0; i < games.size(); i++) {
			total_winnings += (i + 1) * games[i].bid;
		}
		return total_winnings;
	}

private:
	enum class Hand : int { None = 0, HighCard, OnePair, TwoPairs, ThreeOfAKind, FullHouse, FourOfAKind, FiveOfAKind };
	struct Game { array<int, 5> cards; uint64_t bid; Hand hand; uint64_t hash; };

	static int cardValue(char card, const string& order) {
		for (int i = 0; i < order.length(); i++) {
			if (card == order[i]) {
				return i;
			}
		}
		return -1;
	}

	static Hand computeHand(array<int, 5>& original_cards, int joker_id = -1) {
		array<int, 5> cards = original_cards;
		sort(cards.begin(), cards.end());
		vector<int> acc;

		int i = 0, jokers = 0;
		while (i < 5) {
			int d = 0;
			while (i + d < 5 && cards[i] == cards[i + d]) d++;
			//there are joker cards in play
			if (joker_id != -1 && cards[i] == joker_id) {
				jokers = d;
			}
			else if (d > 1) {
				acc.emplace_back(d);
			}
			i += d;
		}

		int hash = 0;
		if (acc.size() > 0) {
			sort(acc.begin(), acc.end(), [](int a, int b) {return a > b; });
			acc.front() += jokers;
			for (int i = 0; i < acc.size(); i++) {
				hash = acc[i] + hash * 10;
			}
		}
		else if (joker_id != -1) {
			if (jokers > 0 && jokers < 5)
				hash = jokers + 1;
			else
				hash = jokers;
		}

		Hand output = Hand::None;
		switch (hash)
		{
		case 0:
			output = Hand::HighCard;
			break;
		case 2:
			output = Hand::OnePair;
			break;
		case 3:
			output = Hand::ThreeOfAKind;
			break;
		case 4:
			output = Hand::FourOfAKind;
			break;
		case 5:
			output = Hand::FiveOfAKind;
			break;
		case 22:
			output = Hand::TwoPairs;
			break;
		case 32:
			output = Hand::FullHouse;
			break;
		default:
			output = Hand::None;
			__debugbreak();
			break;
		}
		return output;
	}

	static vector<Game> computeGames(vector<string> lines, const string& order, const int& joker_id = -1) {
		vector<Game> games;
		for (auto& line : lines) {
			Game game;
			auto info = AoC::parseString(line, " ", true);
			for (int i = 0; i < 5; i++) {
				game.cards[i] = cardValue(info[0][i], order);
			}
			game.hand = computeHand(game.cards, joker_id);
			game.hash = static_cast<uint64_t>(game.hand);
			for (int i = 0; i < 5; i++) {
				game.hash = game.cards[i] + game.hash * 100;
			}
			game.bid = stoull(info[1]);
			games.emplace_back(game);
		}
		// rank games
		sort(games.begin(), games.end(), [](const Game& a, const Game& b) {return (a.hash < b.hash); });
		return games;
	}


};

int AoC_D7::solve() {
	auto lines = AoC::readFile("./src/D7_Camel_Cards/input.txt");
	if (lines.empty()) return 1;
	for (auto& line : lines) {
		cout << line << "\r\n";
	}
	cout << "\r\n";

	Solution solution;
	std::cout << "-----------------------------------------------------\r\n";
	std::cout << "Part 1 = " << solution.solvePart1(lines) << "\r\n"; // 251136060
	std::cout << "Part 2 = " << solution.solvePart2(lines) << "\r\n"; // 249400220
	std::cout << "-----------------------------------------------------\r\n";



	return 0;
}
