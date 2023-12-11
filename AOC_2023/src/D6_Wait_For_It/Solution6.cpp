#include "pch.h"
#include "Solution6.h"

namespace AoC_D6 {
	using namespace std;
	class Solution;
}

class AoC_D6::Solution {
public:

	uint64_t solvePart1(const vector<string>& lines) {
		auto timings = parseMultipleData(0, lines);
		auto distances = parseMultipleData(1, lines);
		uint64_t answer = 1;
		for (size_t i = 0; i < timings.size(); i++) {
			uint64_t invalid_races = invalidRaces(timings[i], distances[i]);
			uint64_t total_races = timings[i] + 1;
			answer *= (total_races - invalid_races);
		}
		return answer;
	}

	uint64_t solvePart2(const vector<string>& lines) {
		auto timing = parseData(0, lines);
		auto distance = parseData(1, lines);
		uint64_t invalid_races = invalidRaces(timing, distance);
		uint64_t total_races = timing + 1;
		return (total_races - invalid_races);
	}

private:
	static vector<uint64_t> parseMultipleData(size_t line_idx, vector<string> lines) {
		vector<uint64_t> output;
		auto vec = AoC::parseString(lines[line_idx], " ", true);
		vec.erase(vec.begin());
		for (auto& elem : vec) {
			output.emplace_back(stoull(elem));
		}
		return output;
	}

	static uint64_t parseData(size_t line_idx, vector<string> lines) {
		auto vec = AoC::parseString(lines[line_idx], " ", true);
		vec.erase(vec.begin());
		string aux;
		for (auto& elem : vec) {
			aux += elem;
		}
		return stoull(aux);
	}

	static uint64_t invalidRaces( uint64_t race_time, uint64_t record_dist) {
		uint64_t time_holding = 0, invalid_races = 0;
		while (time_holding <= race_time) {
			uint64_t velocity = time_holding;
			uint64_t time = race_time - time_holding;
			uint64_t dist = velocity * time;

			if (dist <= record_dist) {
				invalid_races++;
			} else {
				invalid_races *= 2;
				break;
			}
			time_holding++;
		}
		return invalid_races;
	}

};

int AoC_D6::solve() {
	auto lines = AoC::readFile("./src/D6_Wait_For_It/input.txt");
	if (lines.empty()) return 1;
	for (auto& line : lines) {
		cout << line << "\r\n";
	}
	cout << "\r\n";

	Solution solution;
	std::cout << "-----------------------------------------------------\r\n";
	std::cout << "Part 1 = " << solution.solvePart1(lines) << "\r\n";
	std::cout << "Part 2 = " << solution.solvePart2(lines) << "\r\n";
	std::cout << "-----------------------------------------------------\r\n";



	return 0;
}
