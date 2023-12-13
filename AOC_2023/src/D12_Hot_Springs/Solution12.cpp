#include "pch.h"
#include "Solution12.h"

namespace AoC_D12 {
	using namespace std;

	static constexpr int MAX_GROUPS = 100;
	static constexpr int DOT = 0;
	static constexpr int HASH = 1;

	typedef struct data_t {
		string pattern;
		vector<int> groups;
		int nHashes;
	} data_t;

	vector<data_t> parseData(const vector<string>& lines) {
		vector<data_t> output;
		for (string line : lines) {
			data_t data;
			auto space = line.find(' ');
			data.pattern = line.substr(0, space);
			auto rest = string_view(line).substr(space + 1);
			data.nHashes = 0;
			while (true) {
				auto comma = rest.find(',');
				auto code = stoi(string(rest.substr(0, comma)));
				data.groups.push_back(code);
				data.nHashes += code;
				if (comma == string_view::npos) break;
				rest = rest.substr(comma + 1);
			}
			output.push_back(data);
		}
		return output;
	}

	data_t unfold(data_t& data, int folds) {
		data_t unfolded;
		auto& pattern = unfolded.pattern;
		auto& groups = unfolded.groups;
		auto& nHashes = unfolded.nHashes;
		pattern = "", groups = {}, nHashes = 0;
		for (int i = folds - 1; i >= 0; i--) {
			pattern += data.pattern;
			if (i > 0)
				pattern += '?';
			groups.insert(groups.end(), data.groups.begin(), data.groups.end());
			nHashes += data.nHashes;
		}
		return unfolded;
	}

	vector<bool> getGroupEnds(int nHashes, vector<int> groups) {
		vector<bool> isGroupEnded(nHashes + 1);
		isGroupEnded[0] = true;

		int groupEnd = 0;
		for (int group : groups) {
			groupEnd += group;
			isGroupEnded[groupEnd] = true;
		}
		return isGroupEnded;
	}

	int64_t count(string pattern, vector<int> groups, int nHashes) {
		auto isGroupEnded = getGroupEnds(nHashes, groups);
		int n = pattern.length();

		// memo[i][hashes][char] = possible ways of placing a {char} at position i 
		// where:
		// * i = index location in the string you are trying to place a character at 
		// * hashes = number of hashes placed already (excluding current attempt)
		// * char = character you are trying to place (DOT or HASH)
		vector<vector<array<int64_t, 2>>> memo(n + 1, vector<array<int64_t, 2>>(nHashes + 1, { 0, 0 }));
		memo[0][0][DOT] = 1;

		for (int i = 1; i <= n; i++) {
			if (pattern[i - 1] != '.') { // Possibilities for hash
				for (int hashes = 1; hashes <= std::min(i, nHashes); hashes++) {
					if (isGroupEnded[hashes - 1]) {// If start of new group
						// Must follow a dot
						memo[i][hashes][HASH] = memo[i - 1][hashes - 1][DOT];
					}
					else {
						// Must follow a hash
						memo[i][hashes][HASH] = memo[i - 1][hashes - 1][HASH];
					}
				}
			}
			if (pattern[i - 1] != '#') {// Possibilities for dot
				for (int hashes = 0; hashes <= std::min(i - 1, nHashes); hashes++) {
					if (isGroupEnded[hashes]) {// If end of a group
						// We can safely place a dot 
						memo[i][hashes][DOT] = memo[i - 1][hashes][DOT] + memo[i - 1][hashes][HASH];
					}
					else {
						// We are not at the end of a group, therefore we can't place a dot!
						// Leave dp as 0 at this position
					}
				}
			}
		}
		auto ans = memo[n][nHashes][DOT] + memo[n][nHashes][HASH];
		return ans;
	}

	int64_t solvePart1(const vector<string>& lines) {
		int64_t sum = 0;
		auto allData = parseData(lines);
		for (auto& data : allData) {
			sum += count(data.pattern, data.groups, data.nHashes);
		}
		return sum;
	}

	int64_t solvePart2(const vector<string>& lines) {
		int64_t sum = 0;
		auto allData = parseData(lines);
		for (auto& data : allData) {
			data = unfold(data, 5);
			sum += count(data.pattern, data.groups, data.nHashes);
		}
		return sum;
	}

}

int AoC_D12::solve() {
	auto lines = AoC::readFile("./src/D12_Hot_Springs/input.txt");
	if (lines.empty()) return 1;

	std::ostringstream oss;
	for (auto& line : lines) {
		oss << line << "\r\n";
	}
	oss << "\r\n";
	cout << oss.str();

	std::cout << "-----------------------------------------------------\r\n";
	std::cout << "Part 1 = " << solvePart1(lines) << "\r\n"; // 7490
	std::cout << "Part 2 = " << solvePart2(lines) << "\r\n"; // 65607131946466
	std::cout << "-----------------------------------------------------\r\n";

	return 0;
}
