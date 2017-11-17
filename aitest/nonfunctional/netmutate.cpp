#include "../ai/NNetworkPrintOutput.hpp"
#include "../ai/NNetwork.hpp"
#include "../ai/Generation.hpp"
#include <iostream>

#include <stdlib.h>
#include <time.h>

#include <vector>
#include <queue>
#include <unordered_map>

using namespace network;

void print_network(NNetwork *net) {
	std::vector<std::vector<Edge>> edges(net->numNodes);
	
	for (auto conn : net->conns) {
		if (conn.enabled)
			edges[conn.startid].push_back(conn);
	}
	
	for (size_t i=0; i<edges.size(); i++) {
		std::cout << i << " | ";
		for (auto conn : edges[i]) {
			std::cout << conn.innov << ":(" << conn.endid << ',' << conn.weight << ") ";
		}
		std::cout << std::endl;
	}		
}

int main (int argc, char** argv) {
	if (argc != 3) {
		std::cout << "Repeatedly mutate a neural network." << std::endl;
        std::cout << "Arguments: [text file] [number of times]" << std::endl;
		std::cout << "ex. neural.txt 20" << std::endl;
        exit(101);
    }
	unsigned int MUTLOOPS = atoi(argv[2]);
	
	srand(time(NULL));
	
	NNetworkPrintOutput po;
	NNetwork net(&po, std::string(argv[1]));
	
	Generation gen(&po);		//this is just for getInnovNum
	for (auto conn : net.conns) {
		gen.curgenes[{conn.startid, conn.endid}] = conn.innov;
		gen.maxinnov = (gen.maxinnov > conn.innov) ? gen.maxinnov : conn.innov;
	}
	
	for (unsigned int i=0; i<MUTLOOPS; i++) {
		std::cout << "-------- "
				  << "Mutation " << i << ": "
				  << net.nodes.size() << " nodes, "
				  << net.conns.size() << " connections "
				  << "--------" << std::endl;
		print_network(&net);
		net.mutate(gen);
	}
	std::cout << "-------- "
				  << "Mutation " << MUTLOOPS << ": "
				  << net.nodes.size() << " nodes, "
				  << net.conns.size() << " connections "
				  << "--------" << std::endl;
	print_network(&net);
	
	
	std::cout << "Checking for duplicate innovation numbers... ";
	bool duplicates = false;
	std::unordered_map<size_t,size_t> innovs;
	for (auto conn : net.conns) {
		if (innovs.count(conn.innov)) {
			std::cout << "(" << conn.startid << ',' << innovs[conn.innov] << ")->" << conn.innov << ' ';
			duplicates = true;
		} else {
			innovs[conn.innov] = conn.startid;
		}
	}
	
	if (!duplicates) {
		std::cout << "all clear!";
	}
	std::cout << std::endl;
	
	//check for cycles: if stuck in an infinite loop
	std::cout << "Checking for cycles... (if the code hangs there's a cycle somewhere) " << std::flush;
	std::queue<NodeType*> queue;
	for (size_t i=net.numInputs; i<net.numInputs+net.numOutputs; i++) {
		queue.push(net.nodes[i]);
	}
	
	while (!queue.empty()) {
		NodeType *node = queue.front();
		for (auto conn : node->connections) {
			queue.push(conn.node);
		}
		queue.pop();
	}
	
	std::cout << "all clear!" << std::endl;
	
	return 0;
}