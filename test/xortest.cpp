#include "../ai/NNetworkPrintOutput.hpp"
#include "../ai/NNetwork.hpp"
#include "../ai/Generation.hpp"

#include <vector>
#include <iostream>
#include <algorithm>

#include <cmath>
using namespace network;

class XORai : public Generation {
	using Generation::Generation;
	public:
		void compete() override;
};

double XORfitness(NNetwork *network) {
	double fitness = 0;
	for (int a=0; a<2; a++) {
		for (int b=0; b<2; b++) {	
			std::vector<double> output = network->calculate({1, a, b});
			fitness += fabs(output[0] - (a^b));

			//std::cout << network << ": " << a << '^' << b << " = " << (a^b) << "(" << output[0] << ")" << std::endl;
		}
	}
	//std::cout << "Fitness = " << fitness << std::endl;
	return fitness;
}

void XORai::compete() {
	for (auto network : networks[0]) {
		network->fitnessValue = XORfitness(network);
	}
	std::sort(networks[0].begin(), networks[0].end(), [](NNetwork *a, NNetwork *b) { return (*a) < (*b); });
}


int main() {
	//3 inputs, one of them always set to 1
	//1 output

	//random generation

	NNetworkPrintOutput po;
	//XORai gen(&po, std::string("networks/test"));
	/*NNetwork net(&po, "test_generation/network1.txt");
	std::cout << XORfitness(&net) << std::endl;
	return 0;
	*/
	XORai gen(&po, std::string("test_generation"));

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
	while (gennum < 100) {
		gen.step();
		std::cout << "--- Generation " << gennum << "---" << std::endl;
		std::cout << "Top fitness: " << gen.networks[0].front()->fitnessValue << std::endl;
		
		double fitness = 0;
		for (auto network : gen.networks[0]) {
			fitness += network->fitnessValue;
		}
		std::cout << fitness << std::endl;

		/*std::cout << "Type a character to continue... ";
		std::cin >> pause;
		std::cout << std::endl;*/
		gennum++;
	}

	//gen.save("networks/test2");
	gen.save("test_generation2");

	return 0;
}