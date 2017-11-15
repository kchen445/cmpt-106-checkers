#include "../ai/Generation.hpp"
#include "../ai/NNetworkPrintOutput.hpp"
#include "../ai/NNetwork.hpp"

#include <vector>
#include <iostream>
#include <stdlib.h>
#include <time.h>

using namespace network;

inline double rand_double() {
	return static_cast<double>(rand()) / static_cast<double>(RAND_MAX);
}

Generation* createRandomGeneration(NNetworkOutputType* outputDevice, unsigned int gensize, unsigned int numInputs, unsigned int numOutputs, std::vector<unsigned int> hidden) {
	//calculate node ids for each layer of the network
	std::vector<unsigned int> layerstart, layerend;
	layerstart.push_back(0);						//input ids
	layerend.push_back(numInputs);	
	unsigned int hiddensum = 0;						//internals: start after outputs
	for(unsigned int i : hidden) {
		layerstart.push_back(numInputs + numOutputs + hiddensum);
		layerend.push_back(numInputs + numOutputs + hiddensum + i);
		hiddensum += i;
	}
	layerstart.push_back(numInputs);				//output ids
	layerend.push_back(numInputs+numOutputs);
	
	Generation* gen = new Generation(outputDevice);
	gen->networks.resize(1);		//add a species vector to the generation, without it going out of scope
	for (unsigned int i=0; i<gensize; i++) {
		//create nodes in network
		NNetwork* net = new NNetwork(outputDevice, numInputs, numOutputs, hiddensum);
		
		//create an edge with random weight between every pair of nodes in adjacent layers
		for (size_t i=0; i<layerstart.size()-1; i++) {
			for (size_t sid=layerstart[i]; sid<layerend[i]; sid++) {
				for (size_t eid=layerstart[i+1]; eid<layerend[i+1]; eid++) {
					net->addConnection(sid, eid, gen->getInnovNum(sid, eid), rand_double()*2-1);
				}
			}
		}
		
		gen->networks[0].push_back(net);
	}
	
	return gen;
}

int main(int argc, char** argv) {
	if (argc < 4) {
		std::cout << "Creates a random generation of specified size and network structure and stores it in test_generation/." << std::endl;
		std::cout << "Arguments: (numNetworks) (inputs) (outputs) (layers...)" << std::endl;
		std::cout << "ex. 5 3 1 2 2" << std::endl;
		exit(101);
	}
	srand(time(NULL));
	
	//Generation configurations
	size_t numNetworks = atoi(argv[1]);
	size_t numInputs   = atoi(argv[2]);
	size_t numOutputs  = atoi(argv[3]);
	
	std::vector<unsigned int> layers;
	for (int i=4; i<argc; i++) {
		layers.push_back(atoi(argv[i]));
	}
	
	std::cout << "Creating " << numNetworks << " networks with the following structure:" << std::endl;
	std::cout << "\tnumInputs: " << numInputs << std::endl;;
	std::cout << "\tnumOutputs: " << numOutputs << std::endl;;
	std::cout << "\tnumHidden: ";
	for (auto i : layers)
		std::cout << i << ' ';
	std::cout << std::endl;
	
	//Create generation
	Generation* gen = createRandomGeneration(new NNetworkPrintOutput{}, numNetworks, numInputs, numOutputs, layers);
	
	//Print out created edges
	for (auto& kv : gen->curgenes) {
		std::cout << "Innov " << kv.second << ": " << kv.first.first << ' ' << kv.first.second << std::endl;	
	}
	
	//Save generation
	gen->save("test_generation");
	delete gen;
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
