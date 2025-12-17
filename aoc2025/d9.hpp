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

struct cord {
	long long x;
	long long y;
};

template<>
struct std::formatter<cord, char>
{
	template<class ParseContext>
	constexpr ParseContext::iterator parse(ParseContext& ctx)
	{
		return ctx.begin();
	}

	template<class FmtContext>
	FmtContext::iterator format(cord s, FmtContext& ctx) const
	{
		std::ostringstream out;
		out << '(' << s.x << ", " << s.y << ')';

		return std::ranges::copy(std::move(out).str(), ctx.out()).out;
	}
};

void d9p2() {
	auto file_lines = to_lines(parse_file("inputs/d9.txt"));
	// wrap around
	file_lines.push_back(file_lines.front());

	std::vector<std::array<cord, 2>> horiz_lines;
	std::vector<std::array<cord, 2>> vert_lines;

	// parse lines
	for (auto&& line : file_lines | std::views::slide(2)) {
		std::array<cord, 2> data_line;
		std::array<long long, 2> tmp;

		for (auto&& [index, nums] : line[0] | std::views::split(',') | std::views::enumerate) {
			std::from_chars(nums.data(), nums.data() + nums.size(), tmp[index]);
		}
		data_line[0] = cord{ tmp[0], tmp[1] };

		for (auto&& [index, nums] : line[1] | std::views::split(',') | std::views::enumerate) {
			std::from_chars(nums.data(), nums.data() + nums.size(), tmp[index]);
		}
		data_line[1] = cord{ tmp[0], tmp[1] };
		if(data_line[0].x == data_line[1].x) {
			vert_lines.push_back(data_line);
		} else {
			horiz_lines.push_back(data_line);
		}
	}

	// parse points
	std::vector<cord> points;
	for (auto& line : file_lines) {
		std::array<long long, 2> point;
		for (auto [index, nums] : line | std::views::split(',') | std::views::enumerate) {
			std::from_chars(nums.data(), nums.data() + nums.size(), point[index]);
		}
		points.push_back(std::bit_cast<cord>(point));
	}

	auto collides = [horiz_lines, vert_lines](const std::array<cord, 2>& line) {
		auto lx_min = std::min(line[0].x, line[1].x);
		auto lx_max = std::max(line[0].x, line[1].x);
		auto ly_min = std::min(line[0].y, line[1].y);
		auto ly_max = std::max(line[0].y, line[1].y);

		// line is horizontal
		if (ly_min == ly_max) {
			auto ly = line[0].y;
			for (auto& h_line : horiz_lines) {
				auto hx_min = std::min(h_line[0].x, h_line[1].x);
				auto hx_max = std::max(h_line[0].x, h_line[1].x);
				auto hy = h_line[0].y;
				if (ly == hy and
					(
						lx_min <= hx_min and hx_min <= lx_max or
						lx_min <= hx_max and hx_max <= lx_min or
						hx_min <= lx_min and lx_max >= hx_max
					)
			
				) {
					return true;
				}
			}

			for (auto& v_line : vert_lines) {
				auto vy_min = std::min(v_line[0].y, v_line[1].y);
				auto vy_max = std::max(v_line[0].y, v_line[1].y);
				auto vx = v_line[0].x;
				if (vy_max >= ly and vy_min <= ly and vx >= lx_min and vx <= lx_max) {
					return true;
				}
			}
		}

		// line is vertical
		if (line[0].x == line[1].x) {
			auto lx = line[0].x;
			for (auto& h_line : horiz_lines) {
				auto hx_min = std::min(h_line[0].x, h_line[1].x);
				auto hx_max = std::max(h_line[0].x, h_line[1].x);
				auto hy = h_line[0].y;
				if (hx_max > lx and hx_min < lx and hy > ly_min and hy < ly_max) {
					return true;
				}
			}

			for (auto& v_line : vert_lines) {
				auto vy_min = std::min(v_line[0].y, v_line[1].y);
				auto vy_max = std::max(v_line[0].y, v_line[1].y);
				auto vx = v_line[0].x;
				if (vx == lx and
					(
						ly_min <= vy_min and vy_min <= ly_max or
						ly_min <= vy_max and vy_max <= ly_min or
						vy_min <= ly_min and ly_max >= vy_max
					)
				) {
					return true;
				}
			}
		}
		return false;
	};
	auto inside_polygon = [vert_lines](const cord& point) {
		int intersections = 0;

		for (auto& v_line : vert_lines) {
			auto vy_min = std::min(v_line[0].y, v_line[1].y);
			auto vy_max = std::max(v_line[0].y, v_line[1].y);
			auto vx = v_line[0].x;
			if (point.y >= vy_min and point.y <= vy_max and vx > point.x) {
				intersections++;
			}
		}
		
		return intersections % 2 == 1;
	};

	long long largest = 0;
	for (size_t i = 0; i < points.size(); i++)
		for (size_t j = i + 1; j < points.size(); j++) {
			// use the rectangle that is directly inside of the two points
			long long min_x = std::min(points[i].x, points[j].x)+1;
			long long max_x = std::max(points[i].x, points[j].x)-1;
			long long min_y = std::min(points[i].y, points[j].y)+1;
			long long max_y = std::max(points[i].y, points[j].y)-1;

			std::array<cord, 2> line1 = { cord{min_x, min_y}, cord{min_x, max_y} };
			std::array<cord, 2> line2 = { cord{min_x, max_y}, cord{max_x, max_y} };
			std::array<cord, 2> line3 = { cord{max_x, max_y}, cord{max_x, min_y} };
			std::array<cord, 2> line4 = { cord{max_x, min_y}, cord{min_x, min_y} };

			long long x = std::abs(points[i].x - points[j].x) + 1;
			long long y = std::abs(points[i].y - points[j].y) + 1;

			long long answer = x * y;
			
			cord point_i = points[i];
			cord point_j = points[j];
			
			if(    collides(line1)
				or collides(line2)
				or collides(line3)
				or collides(line4)) {
				continue;
			}

			cord midpoint = cord{ (point_i.x + point_j.x) / 2, (point_i.y + point_j.y) / 2 };
			if(!inside_polygon(midpoint)) {
				continue;
			}

			largest = std::max(largest, answer);
		}
	std::println("{}", largest);
}
