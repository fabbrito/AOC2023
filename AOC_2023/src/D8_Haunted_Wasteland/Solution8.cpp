#include "pch.h"
#include "Solution8.h"

namespace AoC_D8 {
	using namespace std;
	class Solution;
}

class AoC_D8::Solution {
public:
	Solution(vector<string> lines) {
		sort(lines.begin() + 2, lines.end());
		m_ids = {};
		m_As = {}, m_Zs = {};
		m_steps = lines.size() - 2;
		m_maps = vector<array<size_t, 2>>(m_steps, { 0, 0 });
		setDirections(lines[0]);
		setIds(lines);
		setMaps(lines);
		sort(m_As.begin(), m_As.end());
		sort(m_Zs.begin(), m_Zs.end());
	}

	uint64_t solvePart1() {
		size_t idx = 0;
		uint64_t count = 0;
		while (count < 100000 && idx != m_steps - 1) {
			singleIter(idx);
			count++;
		}
		return count * m_directions.size();
	}

	uint64_t solvePart2() {
		size_t m = m_As.size(), n = m_Zs.size();

		vector<size_t> indexes = m_Zs;
		vector<uint64_t> counts(m, 0);

		for (size_t i = 0; i < n; i++) {
			for (size_t j = 0; j < m; j++) {
				if (counts[j] != 0)
					continue;
				size_t idx = m_As[j];
				uint64_t count = 0;
				while (count < 100000) {
					singleIter(idx);
					count++;
					if (idx == m_Zs[i]) {
						counts[j] = count;
						//cout << m_As[j] << " " << m_Zs[i] << " -> " << idx << " " << count << endl;
						break;
					}
				}
			}
		}

		uint64_t answer = m_directions.size();
		for (const uint64_t& value : counts) {
			answer = std::lcm(value, answer);
		}
		return answer;
	}
private:
	map<string, size_t> m_ids;
	vector<size_t> m_As;
	vector<size_t> m_Zs;
	vector<array<size_t, 2>> m_maps;
	vector<size_t> m_directions;
	size_t m_steps;

	void singleIter(size_t& idx) {
		for (auto& dir : m_directions) {
			idx = m_maps[idx][dir];
		}
	}

	void multipleIter(vector<size_t>& indexes) {
		for (auto& dir : m_directions) {
			for (auto& idx : indexes) {
				idx = m_maps[idx][dir];
			}
		}
	}

	void setIds(const vector<string>& lines) {
		for (size_t idx = 0; idx < lines.size() - 2; idx++) {
			string id_str = lines[idx + 2].substr(0, 3);
			m_ids[id_str] = idx;
			if (id_str.back() == 'A') {
				m_As.emplace_back(idx);
			}
			else if (id_str.back() == 'Z') {
				m_Zs.emplace_back(idx);
			}
		}
	}

	void setMaps(const vector<string>& lines) {
		for (size_t idx = 2; idx < lines.size(); idx++) {
			string left_str = lines[idx].substr(7, 3);
			string right_str = lines[idx].substr(12, 3);
			m_maps[idx - 2] = { m_ids[left_str], m_ids[right_str] };
		}
	}

	void setDirections(string line) {
		m_directions = vector<size_t>(line.length(), -1);
		for (size_t i = 0; i < line.length(); i++) {
			if (line[i] == 'L') {
				m_directions[i] = 0;
			}
			else if (line[i] == 'R') {
				m_directions[i] = 1;
			}
		}
	}
};

int AoC_D8::solve() {
	auto lines = AoC::readFile("./src/D8_Haunted_Wasteland/input.txt");
	if (lines.empty()) return 1;
	for (auto& line : lines) {
		cout << line << "\r\n";
	}
	cout << "\r\n";

	Solution solution(lines);
	std::cout << "-----------------------------------------------------\r\n";
	std::cout << "Part 1 = " << solution.solvePart1() << "\r\n"; // 16409
	std::cout << "Part 2 = " << solution.solvePart2() << "\r\n"; // 11795205644011
	std::cout << "-----------------------------------------------------\r\n";


	return 0;
}
