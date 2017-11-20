#pragma once

#include "NNetwork.hpp"

#include <fstream>
#include <cstdlib>
#include <cmath>
#include <algorithm>

using namespace network;

/* ----------------------------------------------
                  sigmoid function
   ---------------------------------------------- */
template<size_t In, size_t Out>
double NNetwork<In,Out>::sigmoid(double value) {
	return tanh(value*2);
	//	1/(1+exp(-4.9*value));
	//return value;
}

/* ----------------------------------------------
                  random functions
   ---------------------------------------------- */

//returns a double between [a,b]
static inline double randDouble(double a, double b) {
	return (b-a) * rand()/RAND_MAX + a;
}

//returns True with (percent) chance
static inline bool randChance (int percent) {
    return (rand() % 100 + 1) <= percent;
}

//returns an unsigned int between [a,b]
static inline size_t randInt (size_t a, size_t b) {
	return (rand() % (b-a+1) + a);
}


/* ----------------------------------------------
     saving, loading, constructors, destructors 
   ---------------------------------------------- */
template<size_t In, size_t Out>
NNetwork<In,Out>::NNetwork(std::vector<size_t> layers) {
	
	size_t curstart = 0;
	size_t curend = In;
	size_t nextstart;
	size_t nextend = In+Out;
	
	auto it = conns.before_begin();
	
	for (size_t i=0; i<layers.size(); i++)
	{				
		nextstart = nextend;
		nextend = nextstart + layers[i];
		//connect the current layer to the next layer
		//	make all edges [curstart, curend) to [nextstart, nextend)
		for (size_t u=curstart; u<curend; u++) {
			it = conns.insert_after(it, std::vector<Edge>());
			for (size_t v=nextstart; v<nextend; v++) {
				it->push_back({u, v, 1, randDouble(-1, 1), true});
			}
		}
		curstart = nextstart;
		curend = nextend;
	}
	
	this->numNodes = nextend;
	this->connsize = numNodes-Out;
	this->biases.resize(numNodes, 0);
	
	//connect last layer to output layer
	nextstart = In;
	nextend = In+Out;
	for (size_t u=curstart; u<curend; u++) {
		it = conns.insert_after(it, std::vector<Edge>());
		for (size_t v=nextstart; v<nextend; v++) {
			it->push_back({u, v, 1, randDouble(-1, 1), true});
		}
	}
}



template<size_t In, size_t Out>
NNetwork<In,Out>::NNetwork(const NNetwork &other)
  :	conns(other.conns),
	biases(other.biases), 
	connsize(other.connsize),
	numNodes(other.numNodes)
{
	// no code
}

template<size_t In, size_t Out>
NNetwork<In,Out>::NNetwork(const std::string &filename)
{
	std::ifstream file(filename);
	if (!file) {
        throw std::runtime_error("NNetwork: cannot open " + filename + " for reading");
	}
	
	// --- parse numbers of nodes ---
	size_t numInputs, numOutputs;
	file >> this->numNodes >> numInputs >> numOutputs >> std::ws;		//(skip whitespace after)
	if (In != numInputs) { throw std::runtime_error("NNetwork: template argument for number of inputs is not the same as provided in " + filename); }
	if (Out != numOutputs) { throw std::runtime_error("NNetwork: template argument for number of outputs is not the same as provided in " + filename); }
	
	// --- parse biases ---
	double bias;
	this->biases.resize(numNodes);
	for (size_t i=0; i<numNodes; i++) {
		file >> bias >> std::ws;
		this->biases[i] = bias;
	}
	
	// --- parse connections ---
	this->connsize = 0;
	size_t curid = -1;
	auto it = conns.before_begin();
	while (file.good()) {
		size_t startid, endid;
		size_t innov;
		double weight;
		bool enabled;
		file >> startid >> endid >> innov >> weight >> enabled >> std::ws;	//skip trailing whitespace too
		
		//start new group of edges if necessary
		if (curid != startid) {
			it = conns.insert_after(it, std::vector<Edge>());
			curid = startid;
			this->connsize++;
		}
		
		it->push_back({startid, endid, innov, weight, enabled});
	}
	file.close();
}

template<size_t In, size_t Out>
void NNetwork<In,Out>::save(const std::string &filename) {
	std::ofstream file(filename);
	if (!file) {
        throw std::runtime_error("NNetwork: cannot open " + filename + " for saving");
	}
	
	// --- write number of nodes ---
	file << numNodes << ' '
	     << In       << ' '
		 << Out      << std::endl;
		 
	// --- write biases ---
	for (double val : biases) {
		file << val << ' ';
	}
	file << std::endl;
	
	// --- write connections ---
	for (auto &group : conns) {
		for (auto &conn : group) {
			file << conn.startid << ' '
				 << conn.endid 	 << ' '
				 << conn.innov   << ' '
				 << conn.weight  << ' '
				 << conn.enabled << std::endl;
		}
	}
	file.close();
}

template<size_t In, size_t Out>
NNetwork<In,Out>::~NNetwork() {
	//beep boop auto self-destruct
}

template<size_t In, size_t Out>
std::shared_ptr<NetworkType<In, Out>> NNetwork<In,Out>::clone() {
	return std::shared_ptr<NetworkType<In, Out>>(new NNetwork<In,Out>(*this));
}

/* ----------------------------------------------
               calculation functions
   ---------------------------------------------- */
template<size_t In, size_t Out>
std::array<double, Out> NNetwork<In,Out>::solve(std::array<double, In> const &input) {
	//values[i] = value of node with id i
	std::vector<double> values(biases);		//pre-apply bias values
	for (size_t i=0; i<In; i++) {			//fill in input values
		values[i] = input[i];
	}
	
	for (auto &group : conns) {
		if (group[0].startid >= In) {		//calculate sigmoid, if value is an internal node
			values[group[0].startid] = sigmoid(values[group[0].startid]);
		}
		
		for (auto &conn : group) {			//push value to connected nodes
			if (conn.enabled)
				values.at(conn.endid) += values.at(conn.startid) * conn.weight;
		}
	}
	
	std::array<double, Out> output;			//move output values to return array
	for (size_t i=0; i<Out; i++) {
		output[i] = sigmoid(values[In+i]);
	}
	return output;
}
	

/*template<size_t In, size_t Out>
void NNetwork<In,Out>::addConnection(std::forward_list<std::vector<Edge>>::iterator it, size_t endid, double weight) {
	//assert(0 <= startid && startid < numNodes);
	//assert(0 <= endid && endid < numNodes);
	
	it->push_back({(*it)[0].startid, endid, 1, weight, true});
}*/

/* ----------------------------------------------
                mutation functions
   ---------------------------------------------- */
template<size_t In, size_t Out>
void NNetwork<In,Out>::tweakWeight(int chance, double range) {
	for (auto &group : conns) {
		for (auto &conn : group) {
			if (randChance(chance)) {
				conn.weight += randDouble(-range, range);
				//if (conn.weight >= 1) conn.weight = 1;
				//else if (conn.weight < -1) conn.weight = -1;
			}
		}
	}
}

template<size_t In, size_t Out>
void NNetwork<In,Out>::randomizeWeight(int chance) {
	for (auto &group : conns) {
		for (auto &conn : group) {
			if (randChance(chance)) {
				conn.weight = randDouble(-1, 1);
			}
		}
	}
}

template<size_t In, size_t Out>
void NNetwork<In,Out>::tweakBias(int chance, double range) {
	for (double &bias : biases) {
		if (randChance(chance)) {
			bias += randDouble(-range, range);
			//if (bias >= 1) bias = 1;
			//else if (bias < -1) bias = -1;
		}
	}
}

template<size_t In, size_t Out>
void NNetwork<In,Out>::randomizeBias(int chance) {
	for (double &bias : biases) {
		if (randChance(chance)) {
			bias = randDouble(-1, 1);
		}
	}
}

template<size_t In, size_t Out>
void NNetwork<In,Out>::mutateNode(int chance) {
	if (randChance(chance)) {
		//pick rand start node
		auto itnode = conns.begin();	
		size_t delta = randInt(0, connsize-1);		//move forward in the linked list
		for (size_t i=0; i<delta; i++) ++itnode;
		
		//pick rand connection of that node
		Edge &conn = (*itnode)[randInt(0, itnode->size()-1)];		
		
		//disable the connection
		conn.enabled = false;
		size_t endid = conn.endid;
		double weight = conn.weight;
		
		//add connection from start node to new node
		itnode->push_back({(*itnode)[0].startid, numNodes, 1, 1, true});
			
		//create new node
		//add connection from new node to end node
		itnode = conns.insert_after(itnode, std::vector<Edge>());	
		itnode->push_back({numNodes, endid, 1, weight, true});
		
		//add node bias
		biases.push_back(0);
		connsize++;
		numNodes++;
	}
}

template<size_t In, size_t Out>
void NNetwork<In,Out>::mutateConnection(int chance, int retries) {
	if (randChance(chance)) {
		for (int i=0; i<retries; i++) {
			//the start node must not be an output
			//the end node must not be an input
			
			//	0 <= x < connsize: conns[x]
			//	    x >= connsize: connsize-x, the x'th output node
			size_t groupstart = randInt(0, connsize-1);					//pick a random node in conns
			size_t groupend = randInt(groupstart+1, connsize-1+Out);	//pick a node after groupstart
			
			//get start node
			auto itstart = conns.begin();
			for (size_t i=0; i<groupstart; i++) ++itstart;
			
			//get id of end node
			size_t endid;
			if (groupend < connsize) {			//endid is in conns[x]
				auto itend = itstart;
				for (size_t i=0; i<groupend-groupstart; i++) ++itend;
				endid = (*itend)[0].startid;
				if (endid < In) continue;	//check that end node is not an input node
			} else {							//endid is an output
				endid = groupend - connsize;
			}
			
			//check that start->end is not already a connection
			bool isfree = true;
			for (auto &conn : *itstart) {
				if (conn.endid == endid) {
					if (conn.enabled) {			//if enabled: then not valid edge
						isfree = false;
						break;
					} else {					//if disabled: reenable edge with new weight
						conn.weight = randDouble(-1, 1);
						conn.enabled = true;
						return;
					}
				}
			}
			
			//add edge if it was not found
			if (isfree) {
				itstart->push_back({(*itstart)[0].startid, endid, 1, randDouble(-1, 1), true});
				//addConnection(itstart, endid, randDouble(-1, 1));
				break;
			}
		}
	}
}