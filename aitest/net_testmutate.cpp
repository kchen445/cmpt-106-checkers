#include "../ai/NNetwork.hpp"
#include "../ai/NNetworkPrint.hpp"

#include <array>
#include <iostream>

#include <stdlib.h>
using namespace network;

int main() {
	NNetwork<3,1> net("networks/subtraction.txt");
	
	//copy constructor
	NNetwork<3,1> net2(net);
	print_network(net2);
	std::cout << "---------------------------------" << std::endl;
	
	//tweak weight
	net2.tweakWeight(100, 100);
	print_network(net2);
	std::cout << "---------------------------------" << std::endl;
	
	//randomize weight -- should expect all numbers to be within [-1,1]
	net2.randomizeWeight(100);
	print_network(net2);
	std::cout << "---------------------------------" << std::endl;
	
	//tweak bias
	net2.tweakBias(100, 100);
	print_network(net2);
	std::cout << "---------------------------------" << std::endl;
	
	//randomize bias -- should expect all numbers to be within [-1,1]
	net2.randomizeBias(100);
	print_network(net2);
	std::cout << "---------------------------------" << std::endl;
	
	//mutate node
	for(size_t i=0; i<5; i++) {
		net2.mutateNode(100);
	}
	print_network(net2);
	std::cout << "---------------------------------" << std::endl;
	
	//mutate connection
	for(size_t i=0; i<10; i++) {
		net2.mutateConnection(100, 50);
	}
	print_network(net2);
	std::cout << "---------------------------------" << std::endl;
	
	//copy constructor test -- should be the same as before
	print_network(net);
	
	net2.save("net_zmutation.txt");
	std::cout << "saved to net_zmutation.txt" << std::endl;
	
	NNetwork<3,1> random(std::vector<size_t>({3}));
	print_network(random);
	random.save("net_zrandom.txt");
	std::cout << "saved to net_zrandom.txt" << std::endl;
	return 0;
}