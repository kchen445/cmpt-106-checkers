#include "elo.hpp"
#include <iostream>

int main() {
	std::cout << elo::update(100, 100, 1) << std::endl;
	std::cout << elo::update(100, 100, 0) << std::endl;
}

