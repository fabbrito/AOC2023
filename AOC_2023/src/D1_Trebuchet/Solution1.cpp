#include "pch.h"
#include "Solution1.h"

namespace AoC_D1 {
	int numberOnlyDigits(const std::string& str) {
		int ns = str.length();
		int i = 0, j = ns - 1;
		while (i < ns && (str[i] < '0' || str[i] > '9')) i++;
		while (j >=0 && (str[j] < '0' || str[j] > '9')) j--;

		if (i >= ns || j < 0) return 0;
		return static_cast<int>(str[i] - '0') * 10 + static_cast<int>(str[j] - '0');
	}

	int numberTextAndDigits(const std::string& str) {
		const std::vector<std::string> digits = { "zero", "one", "two", "three", "four", "five", "six", "seven", "eight", "nine" };
		int ns = str.length();
		int left_digit = 0;
		bool found = false;
		for (int i = 0; !found && i < ns; i++) {
			if (str[i] >= '0' && str[i] <= '9') {
				left_digit = static_cast<int>(str[i] - '0');
				found = true;
				break;
			}
			for (int index = 0; index < digits.size(); index++) {
				int nd = digits[index].length();
				int j = 0;
				while (i + j < ns && j < nd && str[i + j] == digits[index][j]) j++;
				if (j == nd) {
					left_digit = index;
					found = true;
					break;
				}
			}
		}

		int right_digit = 0;
		found = false;
		for (int i = ns - 1; !found && i >= 0; i--) {
			if (str[i] >= '0' && str[i] <= '9') {
				right_digit = static_cast<int>(str[i] - '0');
				found = true;
				break;
			}
			for (int index = 0; index < digits.size(); index++) {
				int nd = digits[index].length();
				int j = 0;
				while (i - j >= 0 && j < nd && str[i - j] == digits[index][nd - 1 - j]) j++;
				if (j == nd) {
					right_digit = index;
					found = true;
					break;
				}
			}
		}
		return left_digit * 10 + right_digit;
	}
}

int AoC_D1::solve()
{
	std::ifstream file("./src/D1_Trebuchet/input.txt");
	if (file.is_open()) {
		std::string line_buffer;
		int sum0 = 0, sum1 = 0;
		while (std::getline(file, line_buffer)) {
			auto n0 = numberOnlyDigits(line_buffer.c_str());
			sum0 += n0;
			auto n1 = numberTextAndDigits(line_buffer.c_str());
			sum1 += n1;
			//std::cout << "Sum = " << sum0 << "\t|\t" << line_buffer.c_str() << " -> " << n0 << "\r\n";
			//std::cout << "Sum = " << sum1 << "\t|\t" << line_buffer.c_str() << " -> " << n1 << "\r\n";

		}
		file.close();
		std::cout << "Only Digits Sum = " << sum0 << "\r\n";
		std::cout << "Text and Digits Sum = " << sum1 << "\r\n";
	}
	return 0;
}
