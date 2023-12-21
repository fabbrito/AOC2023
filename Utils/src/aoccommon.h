#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>

namespace aoc {
	std::vector<std::string> readFile(const std::string& file_name);
	std::vector<std::string> parseString(std::string& str, const std::string& delimiter, bool ignoreEmpty = false);
	void prettyPrint(std::vector<std::string> lines, bool precise = true, int maxSpaces = 5);

	namespace math {
		// Closed interval [lb, rb]
		template<typename T>
		struct interval {
			T lb;
			T rb;

			static inline auto emptyInterval() { return interval{ std::numeric_limits<T>::max(), std::numeric_limits<T>::min()}; }
			[[nodiscard]] auto empty() const { return lb > rb; }
			[[nodiscard]] auto contains(T v) const { return lb <= v && v <= rb; }
			[[nodiscard]] auto intersects(interval const& rhs) const {
				return !rhs.empty() && (contains(rhs.lb) || contains(rhs.rb) || rhs.contains(lb) || rhs.contains(rb));
			}
			[[nodiscard]] inline auto width() const { return empty() ? 0 : rb - lb + 1; }

			[[nodiscard]] inline auto operator&(interval const& rhs) const {
				if (empty() || rhs.empty()) { return interval::emptyInterval(); }
				if (!intersects(rhs)) { return interval::emptyInterval(); }
				return interval{ std::max(lb, rhs.lb), std::min(rb, rhs.rb) };
			}

			[[nodiscard]] inline auto operator|(interval const& rhs) const {
				if (empty()) { return rhs; }
				if (rhs.empty()) { return *this; }
				return interval{ std::min(lb, rhs.lb), std::max(rb, rhs.rb) };
			}

			[[nodiscard]] inline auto asTuple() const { return std::tuple{ lb, rb }; }
		};
	}

}