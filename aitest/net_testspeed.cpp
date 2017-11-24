#include "benchmark.hpp"
#include "../ai/NNetwork.hpp"
#include "../ai/NNetworkPrint.hpp"

#include <vector>
#include <array>
#include <stdlib.h>
#include <iostream>

int main() {
	NNetwork<5,5> sample(std::vector<size_t>({3,5,4,1,6}));
	
	int temp;
	benchmark::start(&temp);
	for (unsigned int i=0; i<1000; i++) {
		(rand() % 2) ? sample.mutateNode(100) : sample.mutateConnection(100);
	}
	std::cout << "mutation: " << benchmark::end(&temp) << " cycles" << std::endl;
	
	//print_network(sample);
	
	std::array<double, 5> inp;
	
	benchmark::start(&temp);
	for (unsigned int i=0; i<1000; i++) {
		inp[0] = i;
		inp[1] = 2*i;
		inp[2] = 3*i;
		inp[3] = 4*i;
		inp[4] = 5*i;
		sample.solve(inp);
	}
	std::cout << "calculation: " << benchmark::end(&temp) << " cycles" << std::endl;
	
	return 0;
}