#include "../ai/NNetworkPrintOutput.hpp"
#include "../ai/Generation.hpp"
#include <iostream>

using namespace network;

int main (int argc, char** argv) {
	Generation gen(new NNetworkPrintOutput{}, "test_generation");
	gen.save("test_generation2");
	
	std::cout << "resaved contents of test_generation in test_generation2" << std::endl;
}