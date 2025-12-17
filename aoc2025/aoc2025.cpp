#include <print>


#include "parsing/parsing.hpp"
#include "d6.hpp"
#include "d7.hpp"
#include "d8.hpp"
#include "d9.hpp"



int main() {
	auto before = std::chrono::high_resolution_clock::now();
	// d6p1();
	// d6p2();
	// d7p1();
	// d7p2();
	// d8p1();
	// d8p2();
	// d9p1();
	d9p2();
	auto after = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double, std::ratio<1,1>> elapsed = after - before;
	std::println("{}", elapsed);
	return 0;
}