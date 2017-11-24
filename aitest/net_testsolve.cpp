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
	for (int i=0; i<5; i++) {
		inps[0] = i; //randInt(0, 10);
		inps[1] = i*2; //randInt(0, 10);
		inps[2] = i; //randInt(0, 10);
		std::cout << inps[0] << " + " << inps[1] << " - 0.5*" << inps[2] << " = ";
		std::cout << net.solve(inps)[0] << std::endl;
	}
	std::cout << std::endl;
	
	//test calculation with biases
	net.biases[3] = 0.5;		//modify output bias to +0.5 -- should see output change
	print_network(net);
	for (int i=0; i<5; i++) {
		inps[0] = i; //randInt(0, 10);
		inps[1] = i*2; //randInt(0, 10);
		inps[2] = i; //randInt(0, 10);
		std::cout << inps[0] << " + " << inps[1] << " - 0.5*" << inps[2] << " = ";
		std::cout << net.solve(inps)[0] << std::endl;
	}
	std::cout << std::endl;
	
	net.biases[4] = -2;		//modify internal node bias to +0.5 -- should see output change
	print_network(net);
	for (int i=0; i<5; i++) {
		inps[0] = i; //randInt(0, 10);
		inps[1] = i*2; //randInt(0, 10);
		inps[2] = i; //randInt(0, 10);
		std::cout << inps[0] << " + " << inps[1] << " - 0.5*" << inps[2] << " = ";
		std::cout << net.solve(inps)[0] << std::endl;
	}
	std::cout << std::endl;
	
	
	//test random network generation
	NNetwork<3,1> netr(std::vector<size_t>({2}));
	netr.save("net_zrandom.txt");
	return 0;
}