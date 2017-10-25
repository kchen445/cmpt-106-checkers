// NOTE: This is a temp test file //

// #include "NNetwork.hpp"
#include "../ai/NNetworkPrintOutput.hpp"
//#include "OutputNode.hpp"
//#include "InputNode.hpp"
//#include "ThresholdNode.hpp"
#include "../ai/NNetwork.hpp"
#include <iostream>

#include <stdlib.h>
#include <time.h>
using namespace network;

void print_vector(const std::vector<double>& v) {
	for (auto i : v) {
		std::cout << i << ' ';
	}
	std::cout << std::endl;
}

int main (int argc, char** argv) {
    if (argc != 2) {
        std::cout << "Error: Invalid number of arguments" << std::endl;
        std::cout << "Number of needed arguments: " << 1 << std::endl;
        exit(101);
    }
	
	srand(time(NULL));

    // neural.txt must be located in the calling directory
    NNetwork neural{new NNetworkPrintOutput{}, std::string(argv[1])};
    // neural.linearize();
    print_vector(neural.calculate({1, 0, -1}));
    print_vector(neural.calculate({-1, -1, -1}));
    print_vector(neural.calculate({69.69, 420.420, 911.911}));
    /*auto outputDevice = new NNetworkPrintOutput{};

    OutputNode o1{};
    ThresholdNode t1{};
    InputNode i1{1};
    InputNode i2{0};
    InputNode i3{-1};

    o1.addConnection(&i1, 0.7);
    o1.addConnection(&t1, 0.4);
    o1.addConnection(&i3, 0.5);
    t1.addConnection(&i2, 0.2);
    t1.addConnection(&i1, 0.6);

    o1.calculate();*/

    fflush(stdout);
	neural.save("neural_out.txt");
}