#pragma once

#include "parsing/parsing.hpp"

#include <print>

void d6p1() {
	std::string bytes = parse_file("inputs/d6.txt");
	auto v = bytes | std::views::split('\n');
	std::vector<std::string> lines = v | std::ranges::to<std::vector<std::string>>();

	if (lines.back().empty()) { lines.pop_back(); }

	std::string last_line = std::move(lines.back());
	lines.pop_back();

	std::vector<std::vector<int>> nums;
	for (auto& line : lines) {
		auto nums_view = line | std::views::chunk_by([](auto l, auto r) {
			return not(std::isdigit((int)r) and std::isspace((int)l));
			});
		nums.push_back({});
		for (auto num : nums_view) {
			auto it = std::ranges::find(num, ' ');
			nums.back().push_back(std::stoi(std::string(num.begin(), it)));
		}
	}
	auto it = std::ranges::remove(last_line, ' ');
	last_line.erase(it.begin(), last_line.end());
	long long answer = 0;
	for (size_t i = 0; i < last_line.size(); i++) {
		if (last_line[i] == '+') {
			for (const auto& numbers : nums) {
				answer += numbers[i];
			}
		}
		else {
			long long sub_answer = 1;
			for (const auto& numbers : nums) {
				sub_answer *= numbers[i];
			}
			answer += sub_answer;
		}
	}

	std::println("{}", answer);
}

void d6p2() {
	std::string bytes = parse_file("inputs/d6.txt");
	auto v = bytes | std::views::split('\n');
	std::vector<std::string> transposed_lines;
	std::string last_line;
	{
		std::vector<std::string> lines = v | std::ranges::to<std::vector<std::string>>();
		if (lines.back().empty()) { lines.pop_back(); }

		transposed_lines.resize(lines[0].size());

		for (auto& tr_str : transposed_lines) {
			tr_str.resize(lines.size() - 1);
		}

		for (size_t x = 0; x < lines.size() - 1; x++) {
			transposed_lines.push_back({});
			for (size_t y = 0; y < lines[0].size(); y++) {
				transposed_lines[y][x] = lines[x][y];
			}
		}
		last_line = std::move(lines.back());
	}


	std::vector<std::vector<int>> nums{ {} };
	for (auto& line : transposed_lines) {
		auto it = std::ranges::remove(line, ' ');
		line.erase(it.begin(), line.end());
		if (line.empty()) {
			nums.push_back({});
			continue;
		}
		nums.back().push_back(std::stoi(line));
	}
	auto it = std::ranges::remove(last_line, ' ');
	last_line.erase(it.begin(), last_line.end());

	long long answer = 0;
	for (size_t i = 0; i < last_line.size(); i++) {
		if (last_line[i] == '+') {
			for (const auto& numbers : nums[i]) {
				answer += numbers;
			}
		}
		else {
			long long sub_answer = 1;
			for (const auto& numbers : nums[i]) {
				sub_answer *= numbers;
			}
			answer += sub_answer;
		}
	}

	std::println("{}", answer);
}
