#include "NNetwork.hpp"
#include "Helper.hpp"
#include <unordered_map>
#include <fstream>
#include <cstdlib>

using namespace network;

NNetwork::NNetwork(NNetworkOutputType* outputDevice)
        : inputs(),
		  outputs(),
		  internals(),
          outputDevice(outputDevice)
{}

NNetwork::NNetwork(NNetworkOutputType* outputDevice, const char *filename)
        : inputs(),
		  outputs(),
		  internals(),
          outputDevice(outputDevice)
{
	std::ifstream file(filename, std::ifstream::in);
	
	// --- parse first line ---
	
	//read numbers of nodes
	//size_t numNodes		//class attribute
	size_t numInputs, numOutputs, numHidden;
	file >> numNodes
	     >> numInputs >> numOutputs >> numHidden >> std::ws;		//(skip whitespace after)
	//construct based on numbers
	for(size_t i=0; i<numInputs ; i++) { inputs.push_back(new InputNode()); }
	for(size_t i=0; i<numOutputs; i++) { outputs.push_back(new OutputNode()); }
	for(size_t i=0; i<numHidden ; i++) { internals.push_back(new ThresholdNode()); }
	
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
		double weight;
		unsigned int innov;
		bool enabled;
		file >> startid >> endid >> weight >> innov >> enabled >> std::ws;	//skip trailing whitespace too
		conns.push_back({startid, endid, weight, innov, enabled});
		
		//addConnection(endid, startid, weight, innov, enabled);
	}
	file.close();
}
		
NNetwork::~NNetwork() {
	//delete all Node*s
	
	//NNetwork.cpp:63:46: warning: deleting object of polymorphic class type 'network::OutputNode' which has non-virtual destructor might cause undefined behavior [-Wdelete-non-virtual-dtor]
	for (auto node : nodes) {
		delete node;
	}
	while(!inputs.empty()) delete inputs.back(), inputs.pop_back();
	while(!outputs.empty()) delete outputs.back(), outputs.pop_back();
	while(!internals.empty()) delete internals.back(), internals.pop_back();
}


NodeType* NNetwork::getNode(size_t id) {
	if (id < inputs.size()) {
		return inputs[id];
	} else if (id-inputs.size() < outputs.size()) {
		return outputs[id-inputs.size()];
	} else {
		return internals.at(id-inputs.size()-outputs.size());
		//at does bound checking
	}
}

size_t NNetwork::addNode() {
	internals.push_back(new ThresholdNode());
	return ++numNodes;
}

/*
void NNetwork::addConnection(size_t endid, size_t startid, double weight, unsigned int innov, bool enabled) {
	//conns.emplace_back(endid, startid, weight, innov, enabled);
	//add connection to list
	conns.push_back({endid, startid, weight, innov, enabled});
	//add it to node, if enabled
	if (enabled) {
		NodeType* end   = getNode(endid);
		NodeType* start = getNode(startid);
		end->addConnection(start, weight);
	}
}

void NNetwork::enableConnection(size_t idx) {
	//enable connection in list
	auto conn = conns.at(idx);
	conn.enabled = true;
	NodeType* end   = getNode(conn.endid);
	NodeType* start = getNode(conn.startid);
	//add connection to node
	end->addConnection(start, conn.weight);
}

void NNetwork::disableConnection(size_t idx) {
	//disable connection in list
	auto conn = conns.at(idx);
	conn.enabled = false;
	NodeType* end   = getNode(conn.endid);
	NodeType* start = getNode(conn.startid);
	//search for connection in node and remove it
	for(auto it = end->connections.begin(); it != end->connections.end(); ++it) {
		if (it->node == start) {
			end->connections.erase(it);
			break;
		}
	}
}*/

std::vector<double> NNetwork::calculate(std::vector<double> const &inputValues) {
    if (inputValues.size() != inputs.size()) {
        throw std::runtime_error("Input value vector does not match number of input nodes.");
    }
	//set up input values, reset neural network
	for (size_t i = 0; i < inputs.size(); ++i)
        inputs[i]->rawValue = inputValues[i];
	for (auto node : internals)
        node->rawValue = 2;
	
	//calculate output nodes and store results
	std::vector<double> outvals;
	for (auto node : outputs) {
        node->calculate();
		outvals.push_back(node->rawValue);
	}
	return outvals;
}
		
void NNetwork::save(const char *filename) {
	std::ofstream file(filename, std::ifstream::out);
	
	// --- write first line ---
	//write number of nodes in network
	file << (inputs.size() + outputs.size() + internals.size()) << ' '
	     << inputs.size()  << ' '
		 << outputs.size() << ' '
		 << internals.size();
	/*for (auto node : internals)		//write types of internal nodes
		file << node->getType();*/
	file << std::endl;
	
	// --- write connections ---
	for (auto conn : conns) {
		file << conn.startid << ' '
		     << conn.endid 	 << ' '
			 << conn.weight  << ' '
			 << conn.innov   << ' '
			 << conn.enabled << std::endl;
	}
	
	file.close();
}

bool containsInnov (Edge* begin, Edge* end, size_t innov) {
	for (; begin < end; ++begin) {
		if (begin->innov == innov) {
			return true;
		}
	}
	return false;
}

void differenceMerge (std::vector<Edge> &diff, 
					  std::vector<Edge> &base, 
					  std::vector<Edge> &ref) 
{

	auto basePtr = &base.front();
	auto refPtr = &ref.front();

	for (; basePtr < &base.back() + 1; ++basePtr) {
		if (!containsInnov(refPtr, &ref.back() + 1, basePtr->innov)) {
			diff.push_back(*basePtr);
		} else {
			refPtr++;
		}
	}
}

bool compareEdge (Edge a, Edge b) {
	return a.innov < b.innov;
}

double NNetwork::difference (NNetwork& other) {
	
	// Number of genes in the larger network.
	size_t N = std::max(conns.size(), other.conns.size());

	size_t E = 0;
	size_t D = 0;

	std::vector<Edge> diff;
	differenceMerge(diff, conns, other.conns);
	differenceMerge(diff, conns, other.conns);

	std::vector<Edge> same;
	auto diffRefPtr = &diff.front();
	for (size_t i = 0; i < conns.size(); ++i) {
		if (!containsInnov(diffRefPtr, &diff.back() + 1, conns[i].innov)) {
			same.push_back(conns[i]);
		}
	}
	diffRefPtr = &diff.front();
	for (size_t i = 0; i < other.conns.size(); ++i) {
		if (!containsInnov(diffRefPtr, &diff.back() + 1, other.conns[i].innov)) {
			same.push_back(conns[i]);
		}
	}
	std::sort(&same.front(), &same.back() + 1, compareEdge);



	size_t disjointPivot = same.back().innov;

	for (size_t i = 0; i < diff.size(); ++i) {
		if (diff[i].innov < disjointPivot) {
			D++;
		} else {
			E++;
		}
	}

	double c1 = 1.0;
	double c2 = 1.0;
	double c3 = 1.0;

	double W = 0;

	for (size_t i = 0; i < same.size(); i += 2) {
		double val = same[i].innov - same[i + 1].innov;
		W += val < 0 ? -val : val;
	}

	W = W / (same.size() / 2);

	return (c1 * double(D))/N + (c2 * double(E))/N + (c3 * W);

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
			mutateAddNoe(i);
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


double NNetwork::difference (NNetwork& other) {

	size_t E = 0;
	size_t D = 0;
	size_t N = 0;


	double W = 0;
	size_t Wcount = 0;

	auto it = this.conns.begin();
	auto jt = other.conns.begin();

	if (it->innov == jt->innov) {
		W += abs(it->weight - jt->weight);
		Wcount++;
		it++;
		jt++;
	} else {
		D++;
		if (it->innov < jt->innov) {
			it++;
		} else {
			jt++;
		}
	}

}


