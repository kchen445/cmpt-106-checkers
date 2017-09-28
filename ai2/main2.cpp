/*
	Demo script which loads, calculates, modifies, then re-saves a neural network
	The network should get progressively larger with each execution
*/

#include <iostream>
#include "NNetwork.hpp"

//RNG
#include <time.h>
#include <stdlib.h>
using namespace network;

int main () {
	// --- load the neural network ---
	NNetwork net("othernet.txt");
	
	// --- calculate the neural network --- 
	//set input values
	for (size_t i=0; i<net.inputs.size(); i++) {
		net.inputs[i]->setValue(!(i & 2));		//1,1,0,0,1,1,0,0,...
	}
	/*net.inputs[0]->setValue(1);
	net.inputs[1]->setValue(1);
	net.inputs[2]->setValue(0);
	net.inputs[3]->setValue(0);
	net.inputs[4]->setValue(1);*/
	
	//calculate
	net.calculate();
	//print outputs
	for (size_t i=0; i<net.outputs.size(); i++) {
		std::cout << "Output Node ["
			  << i << "] "
			  << net.outputs[i]->activate()
			  << std::endl;
	}
	
	// --- add a new neuron to the network ---
	srand(time(NULL));
	
	ThresholdNode* newnode = new ThresholdNode();	//new internal neuron
	net.internals.push_back(newnode);
	
	//randomly connect input nodes to new neuron
	for (NodeType<double>* node : net.inputs) {
		if (rand() % 5 == 0)
			node->addConnection(newnode, rand()/(float)RAND_MAX * 2 - 1);
	}
		
	//randomly connect at least 1 internal neuron
	NodeType<double>* must_connect = net.internals[rand() % net.internals.size()];
	for (NodeType<double>* node : net.internals) {
		if ((node == must_connect) || (rand() % 5 == 0))
			node->addConnection(newnode, rand()/(float)RAND_MAX * 2 - 1);
	}
	
	//randomly connect at least 1 output neuron
	must_connect = net.outputs[rand() % net.outputs.size()];
	for (NodeType<double>* node : net.outputs) {
		if ((node == must_connect) || (rand() % 5 == 0))
			newnode->addConnection(node, rand()/(float)RAND_MAX * 2 - 1);
	}
	
	// --- save the modified network ---
	net.save("othernet.txt");
}