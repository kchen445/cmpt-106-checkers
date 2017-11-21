#include "../ai/NNetwork.hpp"

#include <array>
#include <iostream>

#include <stdlib.h>
using namespace network;

int main() {
	NNetwork<64,32> net(std::vector<size_t>(0));
	net.save("random_network.txt");
	return 0;
}