#include "aoccommon.h"

std::vector<std::string> AoC::readFile(const std::string& file_name) {
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

std::vector<std::string> AoC::parseString(std::string& str, const std::string& delimiter, bool ignoreEmpty) {
	size_t pos = 0;
	std::vector<std::string> output;
	while ((pos = str.find(delimiter)) != std::string::npos) {
		if (!ignoreEmpty || pos != 0) {
			output.emplace_back(str.substr(0, pos));
		}
		str.erase(0, pos + delimiter.length());
	}
	output.emplace_back(str);
	return output;
}
