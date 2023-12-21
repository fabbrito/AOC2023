#include "pch.h"
#include "Solution19.h"

namespace AoC2023_D19 {
	using namespace std;

	// --------------------------- Types ----------------------------------
	using interval_t = aoc::math::interval<int64_t>;

	template <typename T>
	struct part_t {
		array<T, 4> ratings{};
		T operator[](size_t i) const { return ratings[i]; }
		T& operator[](size_t i) { return ratings[i]; }
	};

	struct rule_t {
		char op = NULL;
		char id = NULL;
		int64_t v = 0;
		string_view wf{};
	};

	using workflows_t = std::unordered_map<std::string_view, std::vector<rule_t>>;

	// -------------------------- Constants -------------------------------
	static const string XMAS = "xmas";

	// -------------------------- Functions -------------------------------
	size_t xmas(const char& c) { return XMAS.find(c); }

	workflows_t parseWorkflows(const vector<string>& lines) {
		workflows_t wfs;
		for (auto& line : lines) {
			if (line.empty()) break;
			auto open = line.find('{');
			auto close = line.find('}');

			string_view token{ line.begin(), line.begin() + open };
			string_view svRule{ line.begin() + open + 1, line.begin() + close };
			vector<rule_t> rules;
			size_t left = 0, right = 0;
			while (left < svRule.size() && right != std::string_view::npos) {
				right = svRule.find_first_of(","sv, left);
				rule_t rl{};
				if (right != std::string_view::npos) {
					string_view r{ svRule.begin() + left, svRule.begin() + right };
					rl.id = r[0];
					rl.op = r[1];
					auto k = 0; while (r[k] != ':') { ++k; }
					rl.v = std::stoll(string{ r.begin() + 2,r.begin() + k });
					rl.wf = string_view{ r.begin() + k + 1, r.end() };
				}
				else {
					rl.op = '!';
					rl.wf = string_view{ svRule.begin() + left, svRule.end() };
				}
				left = right + 1;
				rules.push_back(rl);
			}
			wfs.insert({ token, rules });
		}
		return wfs;
	}

	vector<part_t<int64_t>> parseParts(const vector<string>& lines) {
		vector<part_t<int64_t>> parts;
		bool partSection = false;
		for (auto& line : lines) {
			if (line.empty()) { partSection = true; continue; }
			if (!partSection) continue;
			string_view svPart{ line.begin() + 1, line.end() - 1 };
			size_t left = 0, right = 0;
			part_t<int64_t> part;
			while (left < svPart.size()) {
				right = svPart.find_first_of(","sv, left);
				if (right != std::string_view::npos) {
					part[xmas(svPart[left])] = std::stoll(string{ svPart.begin() + left + 2,svPart.begin() + right });
					left = right + 1;
				}
				else {
					part[xmas(svPart[left])] = std::stoll(string{ svPart.begin() + left + 2,svPart.end() });
					break;
				}
			}
			parts.push_back(part);
		}
		return parts;
	}

	bool processPart(part_t<int64_t> part, const workflows_t& wfs) {
		string_view token{ "in" };
		while (token != "A"sv && token != "R"sv) {
			for (auto& rl : wfs.at(token)) {
				if ((rl.op == '!') || (rl.op == '<' && part[xmas(rl.id)] < rl.v) || (rl.op == '>' && part[xmas(rl.id)] > rl.v)) {
					token = rl.wf;
					break;
				}
			}
		}
		return token == "A"sv;
	}

	void processRange(const part_t<interval_t>& p, const workflows_t& wfs, string_view token, size_t rlIdx, auto& sum) {
		if (token == "A"sv) {
			sum += std::transform_reduce(p.ratings.begin(), p.ratings.end(), 1ll, std::multiplies{}, std::mem_fn(&interval_t::width));
			return;
		}
		else if (token == "R"sv) {
			return;
		}

		auto& rl = wfs.at(token)[rlIdx];
		if (rl.op == '!') {
			processRange(p, wfs, rl.wf, 0, sum);
		}
		else {
			auto id = xmas(rl.id);
			auto pOk = p;
			pOk[id] = p[id] & (rl.op == '<' ? interval_t{ p[id].lb,rl.v - 1 } : interval_t{ rl.v + 1, p[id].rb });
			processRange(pOk, wfs, rl.wf, 0, sum);

			auto pFail = p;
			pFail[id] = p[id] & (rl.op == '<' ? interval_t{ rl.v, p[id].rb } : interval_t{ p[id].lb, rl.v });
			processRange(pFail, wfs, token, rlIdx + 1, sum);
		}
	}

	// --------------------------- Part 1 ---------------------------------
	long long solvePart1(const vector<string>& lines) {
		auto wfs = parseWorkflows(lines);
		auto parts = parseParts(lines);
		auto sumRatings = 0ll;
		for (auto& part : parts) {
			if (processPart(part, wfs))
				sumRatings += std::reduce(part.ratings.begin(), part.ratings.end());
		}
		return sumRatings;
	}

	// --------------------------- Part 2 ---------------------------------
	long long solvePart2(const vector<string>& lines) {
		auto wfs = parseWorkflows(lines);
		interval_t iv = { 1, 4000 };
		auto allRatings = 0ll;
		processRange(part_t<interval_t>{iv, iv, iv, iv}, wfs, "in"sv, 0, allRatings);
		return allRatings;
	}
}

int AoC2023_D19::solve() {
#if 0 // tests
	auto lines = aoc::readFile("./src/D19_Aplenty/small.txt");
	if (lines.empty()) return 1;
#else
	auto lines = aoc::readFile("./src/D19_Aplenty/input.txt");
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
	std::cout << "Part 1 = " << part1 << "\r\n"; // 368964
	std::cout << "Part 2 = " << part2 << "\r\n"; // 127675188176682
	std::cout << "-----------------------------------------------------\r\n";
	return 0;
}
