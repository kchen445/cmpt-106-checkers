#include "../ai/NNetwork.hpp"

#include <vector>
#include <iostream>
#include <string>
#include <stdlib.h>
#include <time.h>

using namespace network;

int main(int argc, char** argv) {
	const size_t numNetworks = 10;
	const size_t numInputs   = 3;
	const size_t numOutputs  = 1;
	const std::vector<size_t> layers({3});
	
	//code starts here
	srand(time(NULL));
	
	std::cout << "Creating " << numNetworks << " networks with the following structure:" << std::endl;
	std::cout << "\tnumInputs: " << numInputs << std::endl;;
	std::cout << "\tnumOutputs: " << numOutputs << std::endl;;
	std::cout << "\tnumHidden: ";
	for (auto i : layers)
		std::cout << i << ' ';
	std::cout << std::endl;
	
	//Create generation
	for (size_t i=0; i<numNetworks; i++) {
		NNetwork<numInputs,numOutputs> neural(layers);
		neural.save("test_generation/network" + std::to_string(i) + ".txt");
	}
	
	return 0;
}

/*unsigned int randint(unsigned int a, unsigned int b) {
	return a + rand() % (b-a+1);
}

NNetwork* createRandomNNetwork(NNetworkOutputType* outputDevice, unsigned int inputs, unsigned int outputs, std::vector<unsigned int> hidden) {
	NNetwork* net = new NNetwork(outputDevice, inputs, outputs);
	
	std::vector<unsigned int> layerstart, layerend;
	layerstart.push_back(0); layerend.push_back(numInputs-1);
	unsigned int curpos = numInputs + numOutputs;
	for(unsigned int i : hidden) {
		layerstart.push_back(curpos);
		layerend.push_back(curpos + i-1);
		curpos += i;
	}
	layerstart.push_back(numInputs); layerend.push_back(numInputs+numOutputs-1);
	
	size_t curinnov = 1;
	for (size_t i=0; i<layerstart.size()-1; i++) {
		for (size_t sid=layerstart[i]; sid<layerend[i]; sid++) {
			for (size_t eid=layerstart[i+1]; eid<layerend[i+1]; eid++) {
				net.addConnection(sid, eid, curinnov++, rand_double());
			}
		}
	}
	
	return net;
}*/
