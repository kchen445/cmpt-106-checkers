#include "../ai/NNetworkPrintOutput.hpp"
#include "../ai/NNetwork.hpp"
#include <iostream>

using namespace network;

int main (int argc, char** argv) {
	if (argc != 3) {
		std::cout << "Calculates the difference between two neural networks." << std::endl;
        std::cout << "Arguments: [text file] [text file]" << std::endl;
		std::cout << "ex. diff1.txt diff2.txt" << std::endl;
        exit(101);
    }
	
	NNetworkPrintOutput po;
	NNetwork net1(&po, std::string(argv[1]));
	NNetwork net2(&po, std::string(argv[2]));
	
	std::cout << "Difference between " << argv[1] << " and " << argv[2] << ": " << net1.difference(net2) << std::endl;
}