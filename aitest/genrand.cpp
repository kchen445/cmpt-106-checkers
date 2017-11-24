#include "../ai/NNetwork.hpp"

#include <iostream>
#include <string>
#include <stdlib.h>
#include <time.h>

using namespace network;

int main(int argc, char** argv) {
	const size_t numNetworks = 10;
	const size_t numInputs   = 3;
	const size_t numOutputs  = 1;
	const std::vector<size_t> layers({3});
	
	//code starts here
	srand(time(NULL));
	
	std::cout << "Creating " << numNetworks << " networks with the following structure:" << std::endl;
	std::cout << "\tnumInputs: " << numInputs << std::endl;;
	std::cout << "\tnumOutputs: " << numOutputs << std::endl;;
	std::cout << "\tnumHidden: ";
	for (auto i : layers)
		std::cout << i << ' ';
	std::cout << std::endl;
	
	//Create generation
	for (size_t i=0; i<numNetworks; i++) {
		NNetwork<numInputs,numOutputs> neural(layers);
		neural.save("test_generation/network" + std::to_string(i) + ".txt");
	}
	
	return 0;
}
