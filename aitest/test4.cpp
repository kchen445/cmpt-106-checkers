#include "NNetwork.hpp"

#include <array>
#include <iostream>

#include <stdlib.h>
using namespace network;

int main() {
	NNetwork<3,1> net(std::vector<size_t>({2}));
	net.save("random_network.txt");
	return 0;
}