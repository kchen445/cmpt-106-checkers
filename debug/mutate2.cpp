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
			std::cout << "(" << conn.endid << ',' << conn.weight << ") ";
		}
		std::cout << std::endl;
	}
}

int main() {
	NNetwork<3,1> net("subtraction.txt");
	print_network(net);
	std::array<double, 3> inps;
	
	for (unsigned int i=0; i<5; i++) {
		inps[0] = randInt(0, 10);
		inps[1] = randInt(0, 10);
		inps[2] = randInt(0, 10);
		std::cout << inps[0] << " + " << inps[1] << " - 0.5*" << inps[2] << " = ";
		std::cout << net.solve(inps)[0] << std::endl;
	}
	std::cout << std::endl;
	
	net.mutateNode(100);
	print_network(net);
	
	for (unsigned int i=0; i<5; i++) {
		inps[0] = randInt(0, 10);
		inps[1] = randInt(0, 10);
		inps[2] = randInt(0, 10);
		std::cout << inps[0] << " + " << inps[1] << " - 0.5*" << inps[2] << " = ";
		std::cout << net.solve(inps)[0] << std::endl;
	}
	std::cout << std::endl;
	
	net.save("identity_copy.txt");
	return 0;
}