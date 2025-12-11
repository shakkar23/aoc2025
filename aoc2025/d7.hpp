#pragma once

#include "parsing/parsing.hpp"
#include <print>
#include <numeric>

void d7p1() {
	auto file_lines = to_byte_lines(parse_file("inputs/d7.txt"));
	size_t data_width = file_lines.at(0).size();
	std::vector<bool> stream(data_width);
	auto start_index = std::distance(file_lines.at(0).begin(), std::ranges::find(file_lines.at(0), 'S'));
	stream[start_index] = true;
	// preprocess
	for (auto& layer : file_lines | std::views::drop(1))
		for (auto& el : layer)
			el = el == '^';

	size_t n_splits = 0;
	for (auto& layer : file_lines | std::views::drop(1)) {
		std::vector<bool> next_stream(stream);
		// not taking the first or last elements, they dont have data
		for (size_t i = 1; i < (data_width - 1); ++i) {
			if (stream[i] and layer[i]) {
				next_stream[i - 1] = true;
				next_stream[i + 0] = false;
				next_stream[i + 1] = true;
				n_splits++;
			}
		}

		stream = std::move(next_stream);
	}
	std::println("{}", n_splits);
}



void d7p2() {
	auto file_lines = to_byte_lines(parse_file("inputs/d7.txt"));
	size_t data_width = file_lines.at(0).size();
	std::vector<size_t> stream(data_width);
	auto start_index = std::distance(file_lines.at(0).begin(), std::ranges::find(file_lines.at(0), 'S'));
	stream[start_index] = 1;
	// preprocess
	for (auto& layer : file_lines | std::views::drop(1))
		for (auto& el : layer)
			el = el == '^';

	for (auto& layer : file_lines | std::views::drop(1)) {
		std::vector<size_t> next_stream(stream);
		// not taking the first or last elements, they dont have data
		for (size_t i = 1; i < (data_width - 1); ++i) {
			if (stream[i] and layer[i]) {
				next_stream[i - 1] = next_stream[i - 1] + stream[i];
				next_stream[i + 0] = 0;
				next_stream[i + 1] = next_stream[i + 1] + stream[i];
			}
		}

		stream = std::move(next_stream);
	}
	std::println("{}", std::accumulate(stream.begin(), stream.end(), 0uz));
}