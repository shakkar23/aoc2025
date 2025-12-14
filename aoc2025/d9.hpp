#pragma once

#include "parsing/parsing.hpp"

#include <print>
#include <ranges>

void d9p1() {
	auto lines = to_lines(parse_file("inputs/d9.txt"));
	std::vector<std::array<long long, 2>> cords;
	for (auto& line : lines) {
		std::array<long long, 2> cord;
		for (auto [index, nums] : line | std::views::split(',') | std::views::enumerate) {
			std::from_chars(nums.data(), nums.data() + nums.size(), cord[index]);
		}
		cords.push_back(cord);
	}
	long long largest = 0;
	for (size_t i = 0; i < cords.size(); i++)
		for (size_t j = i+1; j < cords.size(); j++) {
			long long x = std::abs(cords[i][0] - cords[j][0])+1;
			long long y = std::abs(cords[i][1] - cords[j][1])+1;
			largest = std::max(largest, x * y);
		}
	std::println("{}", largest);
}