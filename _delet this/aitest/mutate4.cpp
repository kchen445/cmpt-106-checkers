#include "NNetwork.hpp"

#include <array>
#include <iostream>

#include <stdlib.h>
using namespace network;

/*double randDouble(double a, double b) {
	return (b-a) * rand()/RAND_MAX + a;
}*/

template <size_t In, size_t Out>
void print_network(const NNetwork<In,Out> &net) {
	std::cout << net.connsize << std::endl;
	for (auto group : net.conns) {
		std::cout << group[0].startid << " | ";
		for (auto conn : group) {
			if (conn.enabled) {
				std::cout << "(" << conn.endid << ',' << conn.weight << ") ";
			}
		}
		std::cout << std::endl;
	}
}

int main() {
	NNetwork<3,1> net("subtraction.txt");
	
	for(size_t i=0; i<10; i++) {
		print_network(net);
		if (randChance(50)) {
			net.mutateConnection(100, 50);
		} else {
			net.mutateNode(100);
		}
	}
	
	print_network(net);
	net.save("identity_copy.txt");
	return 0;
}