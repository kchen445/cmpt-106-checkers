#include "../ai/NNetwork.hpp"
#include "../ai/printNetwork.hpp"

//#include <array>
#include <vector>
#include <iostream>

using namespace network;

int main() {
	//simple loading test
	//tests simple calculation
	//tests bias
	//tests saving
	
	std::cout << "---- Identity test ----" << std::endl;
	NNetwork<1,1> net("networks/identity.txt");
	print_network(net);
	std::cout << "-----------------------" << std::endl;
	
	std::vector<double> values;
	for (double val = -1; val < 1.25; val += 0.25) {
		values.push_back(net.solve({val})[0]);
		std::cout << "sigmoid(" << val << ") \t= " << values.back() << std::endl;
	}
	std::cout << std::endl;
	
	
	net.biases[1] = -0.25;
	std::cout << "with an output bias of -0.25:" << std::endl;
	int i=-1;
	for (double val = -1; val < 1.25; val += 0.25) {
		double ans = net.solve({val})[0];
		std::cout << "sigmoid(" << val << "-0.25) \t= " << ans;
		if (i >= 0) {
			if (values[i] == ans) {
				std::cout << " \t== ";
			} else {
				std::cout << " \t!= ";
			}
			std::cout << "sigmoid(" << val-0.25 << ")";
		}
		std::cout << std::endl;
		i++;
	}
	std::cout << std::endl;
	
	std::cout << "-----------------------" << std::endl;
	print_network(net);
	
	net.save("identity_copy.txt");
	std::cout << "check identity_copy.txt for same network except one bias is set to -0.25" << std::endl;
	return 0;
}