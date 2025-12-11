#pragma once

#include "parsing/parsing.hpp"
#include <print>
#include <numeric>
#include <ranges>
#include <charconv>
#include <map>

void d8p1() {
	constexpr size_t top_n_sizes = 3;
	constexpr size_t first_n_closest_junctions = 1000;
	auto lines = to_lines(parse_file("inputs/d8.txt"));
	std::vector<std::array<double, 3>>nums;
	nums.reserve(lines.size());

	for (const auto& line : lines) {
		nums.push_back({});
		for (const auto [i, num_str] : line | std::views::split(',') | std::views::enumerate) {
			std::from_chars(num_str.data(), num_str.data() + num_str.size(), nums.back().at(i));
		}
	}
	std::vector<std::pair<double, std::pair<size_t, size_t>>> distance_to_num_index;

	for (size_t i = 0; i < nums.size(); ++i) {
		for (size_t j = i + 1; j < nums.size(); ++j) {
			auto& a = nums[i];
			auto& b = nums[j];

			double d1 = (a[0] - b[0]);
			double d2 = (a[1] - b[1]);
			double d3 = (a[2] - b[2]);

			d1 *= d1;
			d2 *= d2;
			d3 *= d3;

			distance_to_num_index.push_back(std::make_pair(std::sqrt(d1 + d2 + d3), std::make_pair(i, j)));
		}
	}
	std::ranges::sort(distance_to_num_index, std::ranges::less{}, &std::pair<double, std::pair<size_t, size_t>>::first);

	distance_to_num_index.resize(first_n_closest_junctions);

	std::vector<std::vector<size_t>> circuits;
	std::vector<size_t> used_juntions;

	for (const auto& [dist, indicies] : distance_to_num_index) {
		const auto [first_index, second_index] = indicies;

		bool contains_first = std::ranges::contains(used_juntions, first_index);
		bool contains_second = std::ranges::contains(used_juntions, second_index);

		if (contains_first and not contains_second) {
			for (auto& circuit : circuits) {
				if (std::ranges::contains(circuit, first_index)) {
					circuit.push_back(second_index);
					used_juntions.push_back(second_index);
				}
			}
		}
		else if (contains_second and not contains_first) {
			for (auto& circuit : circuits) {
				if (std::ranges::contains(circuit, second_index)) {
					circuit.push_back(first_index);
					used_juntions.push_back(first_index);
				}
			}
		}
		else if (not contains_first and not contains_second) {
			circuits.push_back({ first_index, second_index });
			used_juntions.push_back(first_index);
			used_juntions.push_back(second_index);
		}
		else /* contains both */ {
			size_t first_junction_index = 5000;
			for (auto [index, circuit] : circuits | std::views::enumerate) {
				if (std::ranges::contains(circuit, first_index)) {
					first_junction_index = index;
				}
			}

			size_t second_junction_index = 5000;
			for (auto [index, circuit] : circuits | std::views::enumerate) {
				if (std::ranges::contains(circuit, second_index)) {
					second_junction_index = index;
				}
			}
			if (first_junction_index == second_junction_index)
				continue;

			// move everything from second to first

			std::vector<size_t> old_circuit = std::move(circuits.at(second_junction_index));

			auto& big_circuit = circuits.at(first_junction_index);

			big_circuit.append_range(old_circuit);

			circuits.erase(std::next(circuits.begin(), second_junction_index));
		}
	}


	std::vector<size_t> circuit_sizes;
	for (const auto& circuit : circuits) {
		circuit_sizes.push_back(circuit.size());
	}
	std::ranges::sort(circuit_sizes, std::ranges::greater{});
	circuit_sizes.resize(top_n_sizes);
	std::println("{}", std::accumulate(circuit_sizes.begin(), circuit_sizes.end(), 1, std::multiplies{}));
}



void d8p2() {
	auto lines = to_lines(parse_file("inputs/d8.txt"));
	std::vector<std::array<double, 3>>nums;
	nums.reserve(lines.size());

	for (const auto& line : lines) {
		nums.push_back({});
		for (const auto [i, num_str] : line | std::views::split(',') | std::views::enumerate) {
			std::from_chars(num_str.data(), num_str.data() + num_str.size(), nums.back().at(i));
		}
	}
	std::vector<std::pair<double, std::pair<size_t, size_t>>> distance_to_num_index;

	for (size_t i = 0; i < nums.size(); ++i) {
		for (size_t j = i + 1; j < nums.size(); ++j) {
			auto& a = nums[i];
			auto& b = nums[j];

			double d1 = (a[0] - b[0]);
			double d2 = (a[1] - b[1]);
			double d3 = (a[2] - b[2]);

			d1 *= d1;
			d2 *= d2;
			d3 *= d3;

			distance_to_num_index.push_back(std::make_pair(std::sqrt(d1 + d2 + d3), std::make_pair(i, j)));
		}
	}
	std::ranges::sort(distance_to_num_index, std::ranges::less{}, &std::pair<double, std::pair<size_t, size_t>>::first);

	std::vector<std::vector<size_t>> circuits;
	std::vector<size_t> used_juntions;
	std::pair<size_t, size_t> last_connector;

	for (const auto& [dist, indicies] : distance_to_num_index) {
		const auto [first_index, second_index] = indicies;

		bool contains_first = std::ranges::contains(used_juntions, first_index);
		bool contains_second = std::ranges::contains(used_juntions, second_index);

		if (contains_first and not contains_second) {
			for (auto& circuit : circuits) {
				if (std::ranges::contains(circuit, first_index)) {
					circuit.push_back(second_index);
					used_juntions.push_back(second_index);
				}
			}
		}
		else if (contains_second and not contains_first) {
			for (auto& circuit : circuits) {
				if (std::ranges::contains(circuit, second_index)) {
					circuit.push_back(first_index);
					used_juntions.push_back(first_index);
				}
			}
		}
		else if (not contains_first and not contains_second) {
			circuits.push_back({ first_index, second_index });
			used_juntions.push_back(first_index);
			used_juntions.push_back(second_index);
		}
		else /* contains both */ {
			size_t first_junction_index = 5000;
			for (auto [index, circuit] : circuits | std::views::enumerate) {
				if (std::ranges::contains(circuit, first_index)) {
					first_junction_index = index;
				}
			}

			size_t second_junction_index = 5000;
			for (auto [index, circuit] : circuits | std::views::enumerate) {
				if (std::ranges::contains(circuit, second_index)) {
					second_junction_index = index;
				}
			}
			if (first_junction_index == second_junction_index) {
				continue;
			}

			// move everything from second to first

			std::vector<size_t> old_circuit = std::move(circuits.at(second_junction_index));

			auto& big_circuit = circuits.at(first_junction_index);

			big_circuit.append_range(old_circuit);

			circuits.erase(std::next(circuits.begin(), second_junction_index));
		}
		last_connector = { first_index, second_index };
	}

	std::vector<std::array<double, 3>> derefs;
	for (const auto& index : { last_connector.first, last_connector.second }) {
		derefs.push_back(nums[index]);
	}
	std::println("{}", derefs[0][0] * derefs[1][0]);
}