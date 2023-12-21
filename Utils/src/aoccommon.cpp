#include "aoccommon.h"

std::vector<std::string> aoc::readFile(const std::string& file_name) {
	std::ifstream file(file_name);
	std::vector<std::string> output;
	if (file.is_open()) {
		std::string line_buffer;
		while (std::getline(file, line_buffer)) {
			output.emplace_back(line_buffer.c_str());
		}
		file.close();
	}
	return output;
}

std::vector<std::string> aoc::parseString(std::string& str, const std::string& delimiter, bool ignoreEmpty) {
	size_t pos = 0;
	std::vector<std::string> output;
	while ((pos = str.find(delimiter)) != std::string::npos) {
		if (!ignoreEmpty || pos != 0) {
			output.emplace_back(str.substr(0, pos));
		}
		str.erase(0, pos + delimiter.length());
	}
	if (!ignoreEmpty || str.length() > 0) {
		output.emplace_back(str);
	}
	return output;
}

void aoc::prettyPrint(std::vector<std::string> lines, bool precise, int maxSpaces) {
	int log10Power = 1, power = 10;
	if (precise)
		maxSpaces = static_cast<int>(std::log10(lines.size())) + 1;
	for (int i = 0; i < lines.size(); i++) {
		if (i == power) {
			power *= 10, log10Power++;
		}
		auto prefix = std::to_string(i) + std::string(maxSpaces - log10Power, ' ') + " | ";
		lines[i].insert(0, prefix);
	}
	std::ostringstream oss;
	for (auto& line : lines) {
		oss << line << std::endl;
	}
	oss << std::endl;
	std::cout << oss.str();
}
