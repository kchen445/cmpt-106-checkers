#include "NNetwork.hpp"
#include "Helper.hpp"
#include <unordered_map>
#include <fstream>
#include <cstdlib>

using namespace network;

NNetwork::NNetwork(NNetworkOutputType* outputDevice)
        : outputDevice(outputDevice)
{}

NNetwork::NNetwork(NNetworkOutputType* outputDevice, const std::string& filename)
        : outputDevice(outputDevice)
{
	std::ifstream file(filename.c_str(), std::ifstream::in);
	
	// --- parse first line ---
	
	//read numbers of nodes
	file >> numNodes >> numInputs >> numOutputs >> std::ws;		//(skip whitespace after)
	for(size_t i=0; i<numInputs ; i++) { nodes.push_back(new InputNode()); }
	for(size_t i=0; i<numOutputs; i++) { nodes.push_back(new OutputNode()); }
	for(size_t i=0; i<numNodes-numInputs-numOutputs; i++) { nodes.push_back(new SigmoidNode()); }
	
	//create nodes from node types
	/*for (size_t i=0; i<num-numInputs-numOutputs; i++) {
		char type;								//read type
		file.get(type);
		switch (type) {
			case 'T':							//threshold node
				nodes.push_back(new ThresholdNode());
				break;
            default:
                throw std::runtime_error("parse error: invalid symbol detected");
		}
	}*/
	
	// --- parse connections ---
	while (file.good()) {
		size_t startid, endid;
		size_t innov;
		double weight;
		bool enabled;
		file >> startid >> endid >> innov >> weight >> enabled >> std::ws;	//skip trailing whitespace too
		addConnection(startid, endid, innov, weight, enabled);
	}
	file.close();
}
	
NNetwork::NNetwork(const NNetwork& other) :
	numInputs(other.numInputs), 
	numOutputs(other.numOutputs),
	numNodes(other.numNodes),
	conns(other.conns) {
	for(size_t i=0; i<numInputs ; i++) { nodes.push_back(new InputNode()); }
	for(size_t i=0; i<numOutputs; i++) { nodes.push_back(new OutputNode()); }
	for(size_t i=0; i<numNodes-numInputs-numOutputs; i++) { nodes.push_back(new SigmoidNode()); }
}

	
void NNetwork::save(const std::string& filename) {
	std::ofstream file(filename.c_str(), std::ifstream::out);
	
	// --- write first line ---
	//write number of nodes in network
	file << numNodes  << ' '
	     << numInputs << ' '
		 << numOutputs << std::endl;
	/*for (auto node : internals)		//write types of internal nodes
		file << node->getType();*/
	
	// --- write connections ---
	for (auto conn : conns) {
		file << conn.startid << ' '
		     << conn.endid 	 << ' '
			 << conn.innov   << ' '
			 << conn.weight  << ' '
			 << conn.enabled << std::endl;
	}
	file.close();
}
		
NNetwork::~NNetwork() {
	//delete all Node*s
	
	//NNetwork.cpp:63:46: warning: deleting object of polymorphic class type 'network::OutputNode' which has non-virtual destructor might cause undefined behavior [-Wdelete-non-virtual-dtor]
	for (auto node : nodes) {
		delete node;
	}
}

size_t NNetwork::addNode() {
	nodes.push_back(new SigmoidNode());
	return ++numNodes;
}

void NNetwork::addConnection(size_t startid, size_t endid, size_t innov, double weight, bool enabled) {
	//conns.emplace_back(endid, startid, weight, innov, enabled);
	conns.push_back({endid, startid, weight, innov, enabled});
	if (enabled) {
		nodes[endid]->addConnection(nodes[startid], weight);
	}
}

void NNetwork::enableConnection(size_t idx) {
	Edge conn = conns.at(idx);
	
	conn.enabled = true;
	nodes[conn.endid]->addConnection(nodes[conn.startid], weight);
}

void NNetwork::disableConnection(size_t idx) {
	//disable connection in list
	Edge conn = conns.at(idx);
	
	conn.enabled = false;
	
	//search for connection in node and remove it
	NodeType* end   = nodes[conn.endid];
	NodeType* start = nodes[conn.startid];
	for(auto it = end->connections.begin(); it != end->connections.end(); ++it) {
		if (it->node == start) {
			end->connections.erase(it);
			break;
		}
	}
}

/*void NNetwork::prepcalc() {
	for(auto node : nodes) {
		node->connections.clear();
	}
	for(auto conn : conns) {
		nodes[conn->endid]->addConnection(nodes[conn->startid], conn->weight);
	}
}*/

std::vector<double> NNetwork::calculate(std::vector<double> const &inputValues) {
    if (inputValues.size() != numInputs) {
        throw std::runtime_error("Input value vector does not match number of input nodes.");
    }
	//set up input values, reset neural network
	for (size_t i = 0; i < numInputs; ++i)
        nodes[i]->rawValue = inputValues[i];
	for (size_t i = numInputs+numOutputs; i < numNodes; i++) {
        node->rawValue = 2;
	
	//calculate output nodes and store results
	std::vector<double> outvals;
	for (size_t i = numInputs; i < numInputs+numOutputs; i++) {
        nodes[i]->calculate();
		outvals.push_back(nodes[i]->rawValue);
	}
	return outvals;
}

double NNetwork::difference(NNetwork& other) {
	//assumes conns are sorted ascending by innovation number
	size_t E = 0, D = 0;
	double Wsum = 0;
	size_t Wcount = 0;

	auto it = this.conns.begin(), iend = this.conns.end();
	auto jt = other.conns.begin(), jend = other.conns.end();

	while (true) {
		if (it->innov == jt->innov) {
			W += abs(it->weight - jt->weight);
			++Wcount;
			++it; ++jt;
		} else {
			++D;
			(it->innov < jt->innov) ? ++it : ++jt;
		}
		
		if (it == iend) {
			while (jt != jend) {++E; ++jt;}
			break;
		} else if (jt == jend) {
			while (it != iend) {++E; ++it;}
			break;
		}
	}
	
	return (c1 * double(E))/numNodes + (c2 * double(D))/numNodes + (c3 * Wsum/Wcount);
}


double rand_double() {
	return static_cast<double>(rand()) / static_cast<double>(RAND_MAX);
}

void mutate () {

	const int weightChangePercent = 20;
	const int addNodeChance = 5;

	for (size_t i = 0; i < conns.size(); ++i) {
		
		if (randomChance(weightChangePercent)) {
			mutateChangeWeightValue(i);
		}

		if (randomChance(addNodeChance)) {
			mutateAddNode(i);
		}

	}

}

void mutateChangeWeightValue (size_t index) {

	const double defaultMax = 0.2;

	double newValue;

	if (randomChance(20)) {
		newValue = 2 * (rand_double() - 0.5);
	} else {

		double crnt = conn[i].weight;

		double maxAdd = std::min(1 - crnt, defaultMax);
		double maxSub = std::max(-1 + crnt, -defaultMax);

		double rand = rand_double();
		newValue = ((maxAdd - maxSub) * rand) + maxSub;
	}

	conns[i].weight = newValue;

}


