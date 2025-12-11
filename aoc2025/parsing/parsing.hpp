#pragma once

#include <fstream>
#include <vector>
#include <ranges>
#include <cstdint>
#include <algorithm>
#include <string>
#include <string_view>
#include <iterator>
#include <filesystem>



std::string parse_file(std::string_view path) {
	std::string bytes;
	std::ifstream file(path.data());
	bytes = [&]() {
		std::string tmp;
		std::string line;
		while (std::getline(file, line)) {
			tmp += line;
			tmp.push_back('\n');
		}
		return tmp;
		}();
	return bytes;
}

std::vector<std::vector<uint8_t>> to_lines(const std::string& str) {
	auto ret = str | std::views::split('\n') | std::ranges::to<std::vector<std::vector<uint8_t>>>();
	while (ret.size() and ret.back().empty()) {
		ret.pop_back();
	}
	return ret;
}