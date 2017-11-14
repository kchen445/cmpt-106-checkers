#include "NNetwork.hpp"

#include <array>
#include <iostream>

#include <stdlib.h>
using namespace network;

/*double randDouble(double a, double b) {
	return (b-a) * rand()/RAND_MAX + a;
}*/

int main() {
	NNetwork<2,1> net("addition.txt");
	std::array<double, 2> inps;
	
	for (unsigned int i=0; i<5; i++) {
		inps[0] = randInt(0, 10);
		inps[1] = randInt(0, 10);
		std::cout << inps[0] << " + " << inps[1] << " = ";
		std::cout << net.solve(inps)[0] << std::endl;
	}
	std::cout << std::endl;
	
	net.save("identity_copy.txt");
	return 0;
}