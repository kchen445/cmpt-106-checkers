#include "../ai/NNetworkPrintOutput.hpp"
#include "../ai/NNetwork.hpp"
#include <iostream>

#include <stdlib.h>
#include <time.h>

using namespace network;

int main (int argc, char** argv) {
	if (argc != 3) {
		std::cout << "Breeds two neural networks together." << std::endl;
        std::cout << "Arguments: [text file] [text file]" << std::endl;
		std::cout << "ex. diff1.txt diff2.txt" << std::endl;
        exit(101);
    }
	
	srand(time(NULL));
	
	NNetworkPrintOutput po;
	NNetwork net1(&po, std::string(argv[1]));
	NNetwork net2(&po, std::string(argv[2]));
	
	NNetwork* off;
	off	= net1.breed(net2);	
	std::cout << argv[1] << " bred with " << argv[2] << ":" << std::endl;
	for (auto conn : off->conns) {
		std::cout << conn.startid << ' '
				  << conn.endid   << ' ' 
				  << conn.innov   << ' '
				  << conn.weight  << ' '
				  << conn.enabled << std::endl;
	}
	delete off;
	
	off = net2.breed(net1);	
	std::cout << argv[2] << " bred with " << argv[1] << ":" << std::endl;
	for (auto conn : off->conns) {
		std::cout << conn.startid << ' '
				  << conn.endid   << ' ' 
				  << conn.innov   << ' '
				  << conn.weight  << ' '
				  << conn.enabled << std::endl;
	}
	delete off;
}