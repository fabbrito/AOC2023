#include "pch.h"
#include "Solution9.h"

namespace AoC_D9 {
	using namespace std;
	class Solution;
}

class AoC_D9::Solution {
public:
	Solution(vector<string> lines) {
		setData(lines);
	}

	int solvePart1() {
		int sum = 0;
		for (vector<int> vec : m_data) {
			auto steps = backwards(vec);
			auto next = nextElement(vec, steps);
			sum += next;
		}
		return sum;
	}

	int solvePart2() {
		int sum = 0;
		for (vector<int> vec : m_data) {
			auto steps = backwards(vec);
			vec.insert(vec.begin(), 0);
			forwards(vec, steps);
			sum += vec.front();
		}
		return sum;
	}
private:
	vector<vector<int>> m_data;

	int nextElement(vector<int>& vec, int steps) {
		int next = 0;
		for (int i = 0; i < steps; i++) {
			next += vec[vec.size() - 1 - i];
		}
		return next;
	}

	int backwards(vector<int>& vec) {
		int n = vec.size();
		bool stop = false;
		int count = 0;
		while (!stop && count < n) {
			for (int i = 0; i < n - 1 - count; i++) {
				vec[i] = vec[i + 1] - vec[i];
			}
			count++;
			stop = std::all_of(vec.begin(), vec.end() - count, [](int i) { return i == 0; });
		}
		return count;
	}

	void forwards(vector<int>& vec, int steps) {
		int n = vec.size();
		for (int s = steps; s > 0; s--) {
			for (int i = n - 1 - s; i >= 0; i--) {
				vec[i] = vec[i + 1] - vec[i];
			}
		}
	}

	void setData(const vector<string>& lines) {
		for (string line : lines) {
			vector<int> aux;
			auto str_vec = AoC::parseString(line, " ", true);
			for (auto& elem : str_vec) {
				aux.emplace_back(stoi(elem));
			}
			m_data.emplace_back(aux);
		}
	}
};

int AoC_D9::solve() {
	auto lines = AoC::readFile("./src/D9_Mirage_Maintenance/input.txt");
	if (lines.empty()) return 1;
	for (auto& line : lines) {
		cout << line << "\r\n";
	}
	cout << "\r\n";

	Solution solution(lines);
	std::cout << "-----------------------------------------------------\r\n";
	std::cout << "Part 1 = " << solution.solvePart1() << "\r\n"; // 2105961943
	std::cout << "Part 2 = " << solution.solvePart2() << "\r\n"; // 1019
	std::cout << "-----------------------------------------------------\r\n";


	return 0;
}
