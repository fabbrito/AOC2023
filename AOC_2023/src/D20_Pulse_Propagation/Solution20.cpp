#include "pch.h"
#include "Solution20.h"

namespace AoC2023_D20 {
	using namespace std;

	enum class MODULE_TYPE { NONE = 0, FLIP_FLOP, CONJUNCTION, BROADCAST };

	struct Module;
	typedef struct conn_t {
		const Module* src = nullptr;
		bool p = false;
	};

	struct Module {
		MODULE_TYPE type{};
		std::string name;
		std::vector<Module*> outputs;

		mutable bool state{ false };
		mutable std::vector<conn_t> inputs;

		void reset() const {
			state = false;
			for (auto& in : inputs) {
				in.p = false;
			}
		}

		std::optional<bool> receive(const Module* src, bool p) const {
			switch (type) {
			case MODULE_TYPE::FLIP_FLOP: {
				if (p == false) {
					return state = !state;
				}
				return {};
			}
			case MODULE_TYPE::CONJUNCTION: {
				auto inputIt = std::ranges::find_if(inputs, [&](const auto& input) { return input.src == src; });
				inputIt->p = p;
				state = std::ranges::any_of(inputs, [](const auto& input) { return input.p == false; });
				return state;
			}
			case MODULE_TYPE::BROADCAST: {
				return {};
			}
			}
		}

	};

	struct Button {
		const Module* broadcast{};
		const Module* rx{};

		std::deque<conn_t> deque;
		std::unordered_map<const Module*, uint64_t> cycles;

		uint64_t hi = 0, lo = 0;

		uint64_t press(auto reps) {
			for (auto turn = 0; turn < reps; ++turn) {
				// dispatches Button::operator() with the turn information
				(*this)(turn);
			}
			return lo * hi;
		}

		uint64_t count() {
			if (rx == nullptr) return 0;
			auto& parent = rx->inputs.front().src;

			for (auto i{ 0UL }; cycles.size() < parent->inputs.size(); ++i) {
				(*this)(i);
			}
			auto lcm{ 1ull };
			for (auto& [_, c] : cycles) {
				lcm = std::lcm(lcm, c);
			}
			return lcm;
		}

		void operator()(uint64_t turn) {
			++lo;
			deque.emplace_back(broadcast, false);

			while (!deque.empty()) {
				conn_t conn = deque.front();
				deque.pop_front();

				if (rx != nullptr && conn.src == (rx->inputs.front()).src) {
					for (auto& [m, p] : conn.src->inputs) {
						if (p == true) {
							cycles.insert({ m, turn + 1 });
						}
					}
				}

				for (auto const* out : conn.src->outputs) {
					hi += (int)conn.p;
					lo += (int)!conn.p;
					if (auto q = out->receive(conn.src, conn.p); q) {
						deque.emplace_back(out, *q);
					}
				}
			}
		}
	};

	std::vector<std::unique_ptr<Module>> parseData(const vector<string>& lines) {
		auto moduleType = [](char c) {
			if (c == '%') { return MODULE_TYPE::FLIP_FLOP; }
			if (c == '&') { return MODULE_TYPE::CONJUNCTION; }
			if (c == 'b') { return MODULE_TYPE::BROADCAST; }
			throw std::runtime_error("unknown module type");
			};

		std::unordered_map<std::string_view, uint64_t> indices;
		std::vector<std::unique_ptr<Module>> modules;

		auto getModule = [&](std::string_view v) -> Module* {
			auto [it, ok] = indices.insert({ v, modules.size() });
			if (ok) {
				modules.push_back(std::make_unique<Module>());
			}
			auto* module = modules[it->second].get();
			module->name = v;
			return module;
			};

		for (auto& line : lines) {
			auto b = line.find(' ');    // arrow: b-> f
			auto f = line.find('>') + 2;

			auto module = getModule(std::string_view{ line.begin() + (size_t)(line[0] != 'b'), line.begin() + b });
			module->type = moduleType(line[0]);

			/*for (const auto& out : std::string_view{ line.begin() + f, line.end() } | std::views::split(',')) {
				std::string_view otherName{ out.begin(), out.end() };
				while (std::isspace(otherName.front())) { otherName.remove_prefix(1); }
				auto* other = getModule(otherName);
				module->outputs.push_back(other);
				other->inputs.emplace_back(module, false);
			}*/

			std::string_view outputs{ line.begin() + f, line.end() };
			size_t left = 0, right = 0;
			while (right != std::string_view::npos && left < outputs.size()) {
				right = outputs.find_first_of(","sv, left);
				std::string_view otherName{};
				if (right != std::string_view::npos)
					otherName = std::string_view{ outputs.begin() + left, outputs.begin() + right };
				else
					otherName = std::string_view{ outputs.begin() + left, outputs.end() };
				while (std::isspace(otherName.front())) { otherName.remove_prefix(1); }
				auto* other = getModule(otherName);
				module->outputs.push_back(other);
				other->inputs.emplace_back(module, false);
				left = right + 1;
			}
		}
		return modules;
	}

	Module* findModule(std::string_view name, const std::vector<std::unique_ptr<Module>>& modules) {
		auto it = std::ranges::find_if(modules, [&](auto const& m) { return m->name == name; });
		if (it == modules.end())
			throw std::runtime_error("module not found");
		return it->get();
	}

	// --------------------------- Part 1 ---------------------------------
	long long solvePart1(const vector<string>& lines) {
		auto modules = parseData(lines);
		const auto* broadcaster = findModule("broadcaster"sv, modules);
		Button btn{ broadcaster,nullptr };
		return btn.press(1000);
	}

	// --------------------------- Part 2 ---------------------------------
	long long solvePart2(const vector<string>& lines) {
		auto modules = parseData(lines);
		const auto* broadcaster = findModule("broadcaster"sv, modules);
		const auto* rx = findModule("rx"sv, modules);
		//std::ranges::for_each(modules, [](auto const& m) { m->reset(); });
		Button btn{ broadcaster,rx };
		return btn.count();
	}

}

int AoC2023_D20::solve() {
#if 0 // tests
	vector<string> lines = {
		"broadcaster -> a, b, c",
		"%a -> b",
		"%b -> c",
		"%c -> inv",
		"&inv -> a",
	};
	auto part1 = solvePart1(lines);
	assert(part1 == 32000000);
	auto part2 = 0;
#elif 0
	vector<string> lines = {
		"broadcaster -> a",
		"%a -> inv, con",
		"&inv -> b",
		"%b -> con",
		"&con -> output",
	};
	auto part1 = solvePart1(lines);
	assert(part1 == 11687500);
	auto part2 = 0;
#else
	auto lines = aoc::readFile("./src/D20_Pulse_Propagation/input.txt");
	if (lines.empty()) return 1;
	auto part1 = solvePart1(lines);
	auto part2 = solvePart2(lines);
#endif
	std::ostringstream oss;
	for (auto& line : lines) {
		oss << line << "\r\n";
	}
	oss << "\r\n";
	std::cout << oss.str();

	std::cout << "-----------------------------------------------------\r\n";
	std::cout << "Part 1 = " << part1 << "\r\n"; // 731517480
	std::cout << "Part 2 = " << part2 << "\r\n"; // 244178746156661
	std::cout << "-----------------------------------------------------\r\n";
	return 0;
	}
