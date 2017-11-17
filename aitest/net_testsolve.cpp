#include "../ai/NNetwork.hpp"
#include "../ai/NNetworkPrint.hpp"

#include <array>
#include <iostream>

#include <stdlib.h>
using namespace network;

int main() {
	//test loading
	NNetwork<3,1> net("networks/subtraction.txt");
	std::array<double, 3> inps;
	
	print_network(net);
	
	//test saving
	net.save("net_zcopy.txt");
	
	//test calculation
	for (unsigned int i=0; i<5; i++) {
		inps[0] = randInt(0, 10);
		inps[1] = randInt(0, 10);
		inps[2] = randInt(0, 10);
		std::cout << inps[0] << " + " << inps[1] << " - 0.5*" << inps[2] << " = ";
		std::cout << net.solve(inps)[0] << std::endl;
	}
	std::cout << std::endl;
	
	//test calculation with biases
	net.biases[1] = 0.5;		//modify output bias to +0.5 -- should see output change
	for (unsigned int i=0; i<5; i++) {
		inps[0] = randInt(0, 10);
		inps[1] = randInt(0, 10);
		inps[2] = randInt(0, 10);
		std::cout << inps[0] << " + " << inps[1] << " - 0.5*" << inps[2] << " = ";
		std::cout << net.solve(inps)[0] << std::endl;
	}
	std::cout << std::endl;
	
	net.biases[3] = 0.5;		//modify internal node bias to +0.5 -- should see output change
	for (unsigned int i=0; i<5; i++) {
		inps[0] = randInt(0, 10);
		inps[1] = randInt(0, 10);
		inps[2] = randInt(0, 10);
		std::cout << inps[0] << " + " << inps[1] << " - 0.5*" << inps[2] << " = ";
		std::cout << net.solve(inps)[0] << std::endl;
	}
	std::cout << std::endl;
	
	
	//test random network generation
	NNetwork<3,1> netr(std::vector<size_t>({2}));
	netr.save("net_zrandom.txt");
	return 0;
}