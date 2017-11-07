#include <iostream>
#include "util.hpp"
#include "NeuralIOSet.hpp"
#include "LearningEntity.hpp"
#include "LearningSet.hpp"
#include "Display.hpp"

namespace test {
	void a () {
		std::cout << __PRETTY_FUNCTION__ << ": hello" << std::endl;
	}
}

int main () {
	test::a();
	std::cout << "OK!" << std::endl;
}
