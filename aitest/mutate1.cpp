#include "NNetwork.hpp"

#include <array>
#include <iostream>

#include <stdlib.h>
using namespace network;

/*double randDouble(double a, double b) {
	return (b-a) * rand()/RAND_MAX + a;
}*/

int main() {
	NNetwork<3,1> net("subtraction.txt");
	std::array<double, 3> inps;
	
	net.tweakBias(100, 50);
	
	for (unsigned int i=0; i<5; i++) {
		inps[0] = randInt(0, 10);
		inps[1] = randInt(0, 10);
		inps[2] = randInt(0, 10);
		std::cout << inps[0] << " + " << inps[1] << " - 0.5*" << inps[2] << " = ";
		std::cout << net.solve(inps)[0] << std::endl;
	}
	std::cout << std::endl;
	
	net.save("identity_copy.txt");
	
	net.randomizeBias(100);
	net.save("identity_copy2.txt");

	
	net.tweakWeight(100, 50);
	net.save("identity_copy3.txt");
	
	net.randomizeWeight(100);
	net.save("identity_copy4.txt");
	
	return 0;
}