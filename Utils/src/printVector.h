#pragma once
#include <iostream>
#include <vector>
#include <string>

namespace utils {
	template <typename T>
	void printVector(const std::vector<T>& vec, std::string separator = ", ", bool endLine = false) {
		std::cout << "{";
		for (std::size_t i = 0; i < vec.size(); i++) {
			if (i < vec.size() - 1) std::cout << vec[i] << separator;
			else std::cout << vec[i];
		}
		std::cout << "}";
		if (endLine) {
			std::cout << std::endl;
		}
			
	}
}
