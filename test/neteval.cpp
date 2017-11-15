#include "../ai/NNetworkPrintOutput.hpp"
#include "../ai/NNetwork.hpp"

#include <iostream>
#include <vector>

#include <stdlib.h>

using namespace network;

void print_vector(const std::vector<double>& v) {
	for (auto i : v) {
		std::cout << i << ' ';
	}
	std::cout << std::endl;
}

int main (int argc, char** argv) {
    //read neural network
	if (argc < 2) {
		std::cout << "Calculates the output of a neural network given some inputs." << std::endl;
		std::cout << "Arguments: [text file] [inputs... (floating point)]" << std::endl;
		std::cout << "ex. neural.txt -1 0.5 1" << std::endl;
        exit(101);
    }
    NNetwork neural{new NNetworkPrintOutput{}, std::string(argv[1])};
	
	//read inputs
	if (argc-2 < neural.numInputs) {
		std::cout << "Error: not enough inputs (required: " << neural.numInputs << ")" << std::endl;
		exit(101);
	}
	std::vector<double> inputs;
	for (size_t i = 2; i < argc; i++) {
		inputs.push_back(atof(argv[i]));
	}
	
	//calculate inputs
	print_vector(neural.calculate(inputs));
	//test calculation
    //print_vector(neural.calculate({1, 0, -1}));					//Output: 
    //print_vector(neural.calculate({-1, -1, -1}));				//Output: 
    //print_vector(neural.calculate({69.69, 420.420, 911.911}));	//Output: 1
    
	//test saving
	//neural.save("neural_out.txt");
}