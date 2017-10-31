#include "../ai/NNetworkPrintOutput.hpp"
#include "../ai/Generation.hpp"

#include <vector>
#include <iostream>
#include <algorithm>

#include <cmath>
using namespace network;

bool network_compare(NNetwork *a, NNetwork *b) {
	return (*a) < (*b);
}

double XORfitness(NNetwork *network) {
	double fitness = 0;
	for (int a=0; a<2; a++) {
		for (int b=0; b<2; b++) {	
			std::vector<double> output = network->calculate({1, a, b});
			//
			fitness += fabs(output[0] - (a^b));

			//std::cout << network << ": " << a << '^' << b << " = " << (a^b) << "(" << output[0] << ")" << std::endl;
		}
	}
	//std::cout << "Fitness = " << fitness << std::endl;
	return fitness;
}

void Generation::compete() {
	for (auto network : networks[0]) {
		network->fitnessValue = XORfitness(network);
	}
	std::sort(networks[0].begin(), networks[0].end(), network_compare);
	/*for (auto network : networks[0]) {
		std::cout << network << std::endl;
	}*/
}


int main() {
	//3 inputs, one of them always set to 1
	//1 output

	//random generation

	NNetworkPrintOutput po;
	Generation gen(&po, std::string("networks/test"));

	/*for (auto network : gen.networks[0]) {
		std::cout << XORfitness(network) << std::endl; 
	}*/

	/*double fitness;	

	gen.step();
	fitness = 0;
	for (auto network : gen.networks[0]) {
		fitness += network->fitnessValue;
	}
	std::cout << fitness << std::endl;

	gen.save("networks/test2");
	gen.step();
	fitness = 0;
	for (auto network : gen.networks[0]) {
		fitness += network->fitnessValue;
	}
	std::cout << fitness << std::endl;

	gen.save("networks/test3");

	return 0;*/

	unsigned int gennum = 0;
	char pause;
	double fitness;
	while (gennum < 10000) {
		gen.step();
		std::cout << "--- Generation " << gennum << "---" << std::endl;
		std::cout << "Top fitness: " << gen.networks[0].front()->fitnessValue << std::endl;
		
		fitness = 0;
		for (auto network : gen.networks[0]) {
			fitness += network->fitnessValue;
		}
		std::cout << fitness << std::endl;

		/*std::cout << "Type a character to continue... ";
		std::cin >> pause;
		std::cout << std::endl;*/
		gennum++;
	}

	gen.save("networks/test2");

	return 0;
}