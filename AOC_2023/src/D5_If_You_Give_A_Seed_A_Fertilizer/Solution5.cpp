#include "pch.h"
#include "Solution5.h"

namespace AoC2023_D5 {
	using namespace std;

	// ---------------------------- Common ----------------------------
	static string consumeLine(vector<string>& lines) {
		string line = lines.front();
		lines.erase(lines.begin());
		return line;
	}

	static uint64_t absDiff(uint64_t a, uint64_t b) {
		return (a > b) ? a - b : b - a;
	}

	struct Range { uint64_t start, end; };
	struct ConversionMap { Range dest, source; };
	struct Data { pair<string, string> fromTo; vector<ConversionMap> ranges; };
	static vector<Data> getData(vector<string>& lines) {
		vector<Data> output;
		while (lines.size() > 0) {
			Data data;
			string name_line = consumeLine(lines);
			string name = aoc::parseString(name_line, " ").front();
			vector<string> fromTo = aoc::parseString(name, "-to-");
			data.fromTo = { fromTo.front(), fromTo.back() };

			while (lines.size() > 0 && lines.front() != "") {
				auto line = consumeLine(lines);
				vector<string> range_s = aoc::parseString(line, " ");

				auto length = stoull(range_s[2]);
				auto dest_start = stoull(range_s[0]);
				auto source_start = stoull(range_s[1]);

				Range dest = { .start = dest_start, .end = dest_start + length - 1 };
				Range source = { .start = source_start ,.end = source_start + length - 1 };

				data.ranges.push_back({ dest,source });
			}
			// sort by source_start
			sort(data.ranges.begin(), data.ranges.end(), [&](ConversionMap a, ConversionMap b) { return a.source.start < b.source.start; });
			output.push_back(data);
			if (lines.size() > 0 && lines.front() == "") consumeLine(lines);
		}
		return output;
	}

	static uint64_t getLowest(vector<uint64_t> vec) {
		uint64_t lowest = UINT64_MAX;
		for (auto& elem : vec) {
			lowest = min(elem, lowest);
		}
		return lowest;
	}

	// ---------------------------- Part 1 ----------------------------
	static vector<uint64_t> getIndividualSeeds(vector<string>& lines) {
		auto line = consumeLine(lines);
		auto seeds_s = aoc::parseString(line, " ");
		seeds_s.erase(seeds_s.begin());
		vector<uint64_t> seeds;
		for (auto& seed_s : seeds_s) {
			seeds.push_back(stoull(seed_s));
		}
		if (lines.size() > 0 && lines.front() == "") consumeLine(lines);
		return seeds;
	}

	static void propagateSeed(uint64_t& location, const vector<Data>& data) {
		for (int step = 0; step < data.size(); step++) {
			auto& ranges = data[step].ranges;
			for (auto& [dest, source] : ranges) {
				if (location >= source.start && location <= source.end) {
					location = dest.start + absDiff(source.start, location);
					break;
				}
			}
		}
	}

	static uint64_t solvePart1(vector<string> lines) {
		auto seeds = getIndividualSeeds(lines);
		auto data = getData(lines);
		vector<uint64_t> locations = seeds;
		for (auto& location : locations) {
			propagateSeed(location, data);
		}

		return getLowest(locations);
	}

	// ---------------------------- Part 2 ----------------------------
	static vector<Range> getSeedRanges(vector<string>& lines) {
		auto line = consumeLine(lines);
		auto seeds_s = aoc::parseString(line, " ");
		seeds_s.erase(seeds_s.begin());
		vector<Range> seeds_vec;
		for (auto i = 0; i < seeds_s.size(); i += 2) {
			auto start = stoull(seeds_s[i]);
			auto length = stoull(seeds_s[i + 1]);
			seeds_vec.push_back({ .start = start, .end = start + length });
		}
		if (lines.size() > 0 && lines.front() == "") consumeLine(lines);
		return seeds_vec;
	}

	static void getDestRanges(Range in, vector<Range>& dest_ranges, const vector<ConversionMap>& ranges, int idx_ranges = 0) {
		if (idx_ranges >= ranges.size()) {
			auto& [dest, source] = ranges[idx_ranges - 1];
			if (in.start >= source.end) {
				dest_ranges.push_back(in);
			}
			return;
		}

		auto& [dest, source] = ranges[idx_ranges];

		Range out = in;
		if (in.start < source.start) {
			if (in.end < source.start) {
				dest_ranges.push_back(out);
			}
			else if (in.end >= source.start && in.end <= source.end) {
				out.end = dest.start + absDiff(source.start, in.end);
				dest_ranges.push_back(out);
			}
			else { // rightOfBounds(in_end)
				// dealing to values to the left of the bounds: [in_start, source_start-1]
				dest_ranges.push_back({ out.start, dest.start - 1 });
				// dealing with values within the bounds: [source_start, source_end]
				dest_ranges.push_back({ dest.start, dest.end });
				// dealing to values to the right of the bounds: [source_end+1, in_end]

				getDestRanges({ source.end + 1, in.end }, dest_ranges, ranges, idx_ranges + 1);
			}
		}
		else if (in.start >= source.start && in.start <= source.end) {
			out.start = dest.start + absDiff(source.start, in.start);

			if (in.end >= source.start && in.end <= source.end) {
				out.end = dest.start + absDiff(source.start, in.end);
				dest_ranges.push_back(out);
			}
			else { // rightOfBounds(in_end)
				// dealing with values within the bounds: [in_start, source_end]
				dest_ranges.push_back({ out.start, dest.end });
				// dealing to values to the right of the bounds: [source_end+1, in_end]
				getDestRanges({ source.end + 1, in.end }, dest_ranges, ranges, idx_ranges + 1);
			}
		}
		else { // rightOfBounds(in_start)
			// go to the next range step
			getDestRanges(in, dest_ranges, ranges, idx_ranges + 1);
		}

	}

	static vector<Range> propagateRange(Range in, const vector<Data>& steps) {
		vector<Range> sources = {}, destinations = { in };
		vector<Range> output = {};
		for (auto& step : steps) {
			sources = destinations;
			// mergeRanges(sources);
			destinations.clear();
			for (auto& source : sources) {
				getDestRanges(source, destinations, step.ranges);
			}

		}
		sort(destinations.begin(), destinations.end(), [&](Range a, Range b) { return a.start < b.start; });
		return destinations;
	}

	static uint64_t solvePart2(vector<string> lines) {
		auto seed_ranges = getSeedRanges(lines);
		auto steps = getData(lines);
		vector<uint64_t> vec;
		for (Range seed_range : seed_ranges) {
			auto location_ranges = propagateRange(seed_range, steps);
			auto lowest = location_ranges.front().start;
			vec.push_back(lowest);
		}

		return getLowest(vec);
	}
}

int AoC2023_D5::solve() {
#if 0 // tests
	auto lines = aoc::readFile("./src/D5_If_You_Give_A_Seed_A_Fertilizer/small.txt");
	if (lines.empty()) return 1;
#else
	auto lines = aoc::readFile("./src/D5_If_You_Give_A_Seed_A_Fertilizer/input.txt");
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
	std::cout << "Part 1 = " << part1 << "\r\n"; // 278755257
	std::cout << "Part 2 = " << part2 << "\r\n"; // 26829166
	std::cout << "-----------------------------------------------------\r\n";

	return 0;
}
