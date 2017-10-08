// NOTE: This is a temp test file //

// #include "NNetwork.hpp"
#include "NNetworkPrintOutput.hpp"
//#include "OutputNode.hpp"
//#include "InputNode.hpp"
//#include "ThresholdNode.hpp"
#include "NNetwork.hpp"
#include <iostream>

using namespace network;

void print_vector(const std::vector<double>& v) {
	for (auto i : v) {
		std::cout << i << ' ';
	}
	std::cout << std::endl;
}

int main () {
    // neural.txt must be located in the calling directory
    NNetwork neural{new NNetworkPrintOutput{}, "neural.txt"};
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