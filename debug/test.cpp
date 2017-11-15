#include "NNetwork.hpp"
#include "NNetwork.cpp"

#include <array>
#include <iostream>

using namespace network;

int main() {
	NNetwork<1,1> net("identity.txt");
	
	for (double val :  net.solve({0.5})) {
		std::cout << val << ' ';
	}
	std::cout << std::endl;
	
	net.save("identity_copy.txt");
	return 0;
}