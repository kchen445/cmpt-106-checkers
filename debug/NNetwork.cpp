#pragma once

#include "NNetwork.hpp"

#include <fstream>
#include <cstdlib>
#include <cmath>
#include <algorithm>

using namespace network;

/*template<size_t In, size_t Out>
NNetwork<In,Out>::NNetwork() : numNodes(In+Out), biases(In+Out, 0)
{
	//no code
}*/

template<size_t In, size_t Out>
double NNetwork<In,Out>::sigmoid(double value) {
    return tanh(value * 2);
//    return value;
}

template<size_t In, size_t Out>
NNetwork<In,Out>::NNetwork(const NNetwork &other) :
	conns(other.conns),
	biases(other.biases), 
	connsize(other.connsize),
	numNodes(other.numNodes) 
{
	//deep copy
	/*for (auto group : conns) {
		other.conns.push_back(std::vector<Edge>(group));
	}*/
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
	file >> numNodes >> numInputs >> numOutputs >> std::ws;		//(skip whitespace after)
	if (In != numInputs) { throw std::runtime_error("NNetwork: template argument for number of inputs is not the same as provided in " + filename); }
	if (Out != numOutputs) { throw std::runtime_error("NNetwork: template argument for number of outputs is not the same as provided in " + filename); }
	
	// --- parse biases ---
	double bias;
	biases.resize(numNodes);
	for (size_t i=0; i<numNodes; i++) {
		file >> bias >> std::ws;
		biases[i] = bias;
	}
	
	// --- parse connections ---
	connsize = 0;
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
			connsize++;
		}
		
		it->push_back({startid, endid, innov, weight, enabled});
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

template<size_t In, size_t Out>
void NNetwork<In,Out>::save(const std::string &filename) {
	std::ofstream file(filename);
	if (!file) {
        throw std::runtime_error("NNetwork: cannot open " + filename + " for saving");
	}
	
	// --- write number of nodes ---
	file << numNodes  << ' '
	     << In << ' '
		 << Out << std::endl;
		 
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
std::array<double, Out> NNetwork<In,Out>::solve(std::array<double, In> const &input) {
	//values[i] = value of node with id i
	std::vector<double> values(biases);		//apply biases
	for (size_t i=0; i<In; i++) {			//fill in input values
		values[i] = input[i];
	}
	
	for (auto &group : conns) {
		//calculate sigmoid, if value is an internal node
		if (group[0].startid >= In) {
			values[group[0].startid] = sigmoid(values[group[0].startid]);
		}
		//push value to connected nodes
		for (auto &conn : group) {
			if (conn.enabled)
				values[conn.endid] += values[conn.startid] * conn.weight;
		}
	}
	
	std::array<double, Out> output;
	for (size_t i=0; i<Out; i++) {
		output[i] = sigmoid(values[In+i]);
	}
	return output;
}

template<size_t In, size_t Out>
void NNetwork<In,Out>::addConnection(std::forward_list<std::vector<Edge>>::iterator it, size_t endid, double weight) {
	//assert(0 <= startid && startid < numNodes);
	//assert(0 <= endid && endid < numNodes);
	
	it->push_back({(*it)[0].startid, endid, 1, weight, true});
}

//returns a double between [0,1]
static inline double randDouble(double a, double b) {
	return (b-a) * rand()/RAND_MAX + a;
}

static inline bool randChance (int percent) {
    return (rand() % 100 + 1) <= percent;
}

static inline size_t randInt (size_t a, size_t b) {
	return (rand() % (b-a+1) + a);
}

template<size_t In, size_t Out>
void NNetwork<In,Out>::tweakWeight(int chance, double range) {
	for (auto &group : conns) {
		for (auto &conn : group) {
			if (randChance(chance)) {
				conn.weight += randDouble(-range, range);
//                if (conn.weight >= 1) conn.weight = 1;
//                else if (conn.weight < -1) conn.weight = -1;
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
			if (bias >= 1) bias = 1;
			else if (bias < -1) bias = -1;
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
		auto itnode = conns.begin();	//pick rand start node
		size_t delta = randInt(0, connsize-1);
		for (size_t i=0; i<delta; i++) ++itnode;
		
		Edge &conn = (*itnode)[randInt(0, itnode->size()-1)];		//pick rand connection of that node
		
		conn.enabled = false;								//disable the connection
		size_t endid = conn.endid;
		double weight = conn.weight;
		
		addConnection(itnode, numNodes, 1);					//add connection from start node to new node
		//itnode->push_back({(*itnode)[0].startid, numNodes, 1, 1, true});
			
		itnode = conns.insert_after(itnode, std::vector<Edge>());	//create new node
		
		itnode->push_back({numNodes, endid, 1, weight, true});
		//addConnection(itnode, conn.endid, conn.weight);	//add connection from new node to end node
		connsize++;
		
		biases.push_back(0);
		numNodes++;

		std::cout << "mutate node" << std::endl;
	}
}

template<size_t In, size_t Out>
void NNetwork<In,Out>::mutateConnection(int chance, int retries) {
	if (randChance(chance)) {
		for (int i=0; i<retries; i++) {
			//pick a start node: cannot be an output
			//pick an end node: cannot be an input
			//(make sure start/end node are not both inputs)
			size_t groupstart = randInt(0, connsize-1);
			size_t groupdelta = randInt(1, (connsize-1)-groupstart+Out);
									   //std::max(In-groupstart, randInt(1, numNodes-1-groupstart));
			
			//get start node in conns
			auto itstart = conns.begin();
			for (size_t i=0; i<groupstart; i++) ++itstart;
			
			size_t endid;
			if (groupdelta >= connsize-groupstart) {
				endid = groupdelta - (connsize-groupstart) + In;
			} else {
				auto itend = itstart;
				for (size_t i=0; i<groupdelta; i++) ++itend;
				endid = (*itend)[0].startid;
				if (endid < In) continue;		//input id
			}
			
			//make sure edge from start->end isn't already created
			bool isfree = true;
			for (auto &conn : *itstart) {
				if (conn.endid == endid) {		//edge found
					if (conn.enabled) {		//if enabled: then not valid edge
						isfree = false;
						break;
					} else {					//if disabled: reenable with new weight
						conn.weight = randDouble(-1, 1);
						conn.enabled = true;
						return;
					}
				}
			}
			
			//add edge if it was not found
			if (isfree) {
				addConnection(itstart, endid, randDouble(-1, 1));
				break;
			}
		}
		std::cout << "mutate connection" << std::endl;
	}
}
