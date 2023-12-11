#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <fstream>

namespace AoC {
	std::vector<std::string> readFile(const std::string& file_name);
	std::vector<std::string> parseString(std::string& str, const std::string& delimiter, bool ignoreEmpty = false);
}