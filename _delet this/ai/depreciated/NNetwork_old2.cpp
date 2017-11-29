#include "NNetwork.hpp"
#include "Helper.hpp"
#include "InputNode.hpp"
#include "OutputNode.hpp"
#include "SigmoidNode.hpp"

#include "Constants.hpp"

#include <unordered_map>
#include <fstream>
#include <cstdlib>

#include <assert.h>

using namespace network;

NNetwork::NNetwork(NNetworkOutputType* outputDevice, size_t numInputs, size_t numOutputs, size_t numHidden)
        : numInputs(numInputs),
		  numOutputs(numOutputs),
		  outputDevice(outputDevice)
{ 
	for(size_t i=0; i<numInputs ; i++) { nodes.push_back(new InputNode()); }
	for(size_t i=0; i<numOutputs; i++) { nodes.push_back(new OutputNode()); }
	for(size_t i=0; i<numHidden; i++) { nodes.push_back(new SigmoidNode()); }
	numNodes = numInputs + numOutputs + numHidden;
}

NNetwork::NNetwork(NNetworkOutputType* outputDevice, const std::string &filename)
        : outputDevice(outputDevice)
{
	std::ifstream file(filename);
	if (!file) {
        throw std::runtime_error("NNetwork: cannot open " + filename + " for reading");
	}
	
	// --- parse numbers of nodes ---
	size_t numHidden;
	file >> numNodes >> numInputs >> numOutputs >> numHidden >> std::ws;		//(skip whitespace after)
	for(size_t i=0; i<numInputs ; i++) { nodes.push_back(new InputNode()); }
	for(size_t i=0; i<numOutputs; i++) { nodes.push_back(new OutputNode()); }
	for(size_t i=0; i<numHidden; i++) { nodes.push_back(new SigmoidNode()); }
	
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
	//conns(other.conns),
	outputDevice(other.outputDevice) {
	for(size_t i=0; i<numInputs ; i++) { nodes.push_back(new InputNode()); }
	for(size_t i=0; i<numOutputs; i++) { nodes.push_back(new OutputNode()); }
	for(size_t i=0; i<numNodes-numInputs-numOutputs; i++) { nodes.push_back(new SigmoidNode()); }
	
	for(auto conn : other.conns) {
		addConnection(conn.startid, conn.endid, conn.innov, conn.weight, conn.enabled);
	}
}

	
void NNetwork::save(const std::string &filename) {
	std::ofstream file(filename);
	if (!file) {
        throw std::runtime_error("NNetwork: cannot open " + filename + " for saving");
	}
	
	// --- write number of nodes ---
	file << numNodes  << ' '
	     << numInputs << ' '
		 << numOutputs << ' '
		 << numNodes-numInputs-numOutputs << std::endl;
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
	for (auto node : nodes) {
		delete node;
	}
}

void NNetwork::precalc() {
	for (auto node : nodes) {
		node->connections.clear();
	}
	
	size_t newnumNodes = 0;
	for(auto conn : conns) {
		newnumNodes = std::max(conn.endid, newnumNodes);
	}
	for(size_t i=numNodes; i<newnumNodes+1; i++) {
		addNode();
	}
	numNodes = newnumNodes+1;
	
	for (auto conn : conns) {
		nodes[conn.endid]->addConnection(nodes[conn.startid], conn.weight);
	}
}

size_t NNetwork::addNode() {
	nodes.push_back(new SigmoidNode());
	return ++numNodes;
}

void NNetwork::addConnection(size_t startid, size_t endid, size_t innov, double weight, bool enabled) {
	//conns.emplace_back(endid, startid, weight, innov, enabled);
	assert(0 <= startid && startid < nodes.size());
	assert(0 <= endid && endid < nodes.size());
	
	conns.push_back({startid, endid, innov, weight, enabled});
	if (enabled) {
		nodes[endid]->addConnection(nodes[startid], weight);
	}
}

void NNetwork::enableConnection(size_t idx) {
	Edge conn = conns.at(idx);
	
	conn.enabled = true;
	nodes[conn.endid]->addConnection(nodes[conn.startid], conn.weight);
}

void NNetwork::disableConnection(size_t idx) {
	Edge conn = conns.at(idx);
	
	conn.enabled = false;					//disable connection in list
	NodeType* end   = nodes[conn.endid];	//search for connection in node and remove it
	NodeType* start = nodes[conn.startid];
	for(auto it = end->connections.begin(); it != end->connections.end(); ++it) {
		if (it->node == start) {
			end->connections.erase(it);
			break;
		}
	}
}

std::vector<double> NNetwork::calculate(std::vector<double> const &inputValues) {
    if (inputValues.size() != numInputs) {
        throw std::runtime_error("NNetwork: Input value vector does not match number of input nodes.");
    }
	
	//set up input values, reset neural network
	for (size_t i = 0; i < numInputs; ++i)
        nodes[i]->rawValue = inputValues[i];
	for (size_t i = numInputs+numOutputs; i < numNodes; i++) {
        nodes[i]->rawValue = 2;
	}
	//calculate output nodes and store results
	std::vector<double> outvals;
	for (size_t i = numInputs; i < numInputs+numOutputs; i++) {
        nodes[i]->calculate();
		outvals.push_back(nodes[i]->rawValue);
	}
	return outvals;
}

double NNetwork::difference(const NNetwork &other) {
	//assumes conns are sorted ascending by innovation number
	//algorithm description:
	//	start two pointers, it/jt at the front of each edge list
	//	when a pointer is moved forward, that edge has been counted
	//	once all edges have been counted, the algorithm is done
	//
	//	if innov numbers of it == jt, move both forward and count one matching pair
	//	if not, move the smaller one forward and count as disjoint
	//	if a pointer can't be moved forward anymore, then the rest are excess
	
	size_t E = 0, D = 0;
	double Wsum = 0;
	size_t Wcount = 0;
	size_t N = (numNodes > other.numNodes) ? numNodes : other.numNodes;
	
	auto it = (*this).conns.begin(), iend = (*this).conns.end();
	auto jt = other.conns.begin(), jend = other.conns.end();
	while (true) {
		if (it->innov == jt->innov) {					//matching
			Wsum += abs(it->weight - jt->weight);		//update weight avg
			++Wcount;
			++it; ++jt;
		} else {										//disjoint
			++D;
			(it->innov < jt->innov) ? ++it : ++jt;
		}
		
		if (it == iend) {								//excess
			while (jt != jend) {++E; ++jt;}
			break;
		} else if (jt == jend) {
			while (it != iend) {++E; ++it;}
			break;
		}
	}
	
	return (c1 * double(E))/N + (c2 * double(D))/N + (c3 * Wsum/Wcount);
}

//assumes this neural network is the more fit one
//	(only add excess or disjoint edges from this network)
NNetwork* NNetwork::breed(const NNetwork &other) {
	assert(numInputs == other.numInputs && numOutputs == other.numOutputs);
	
	NNetwork* off = new NNetwork(outputDevice, numInputs, numOutputs);

	auto it = (*this).conns.begin(), iend = (*this).conns.end();
	auto jt = other.conns.begin(), jend = other.conns.end();
	while (true) {
		if (it->innov == jt->innov) {					//matching
			off->conns.push_back((rand() % 2) ? *it : *jt);
			++it; ++jt;
		} else if (it->innov < jt->innov) {				//this network is disjoint
			off->conns.push_back(*it);
			++it;
		} else {										//other network is disjoint
			//off->conns.push_back(jt);
			++jt;
		}
		
		if (it == iend) {								//other network has excess
			//while (jt != jend) {off->conns.push_back(jt); ++jt;}
			break;
		} else if (jt == jend) {						//this network has excess
			while (it != iend) {off->conns.push_back(*it); ++it;}
			break;
		}
	}
	
	off->precalc();		//make node structure for conns list
	return off;
}

static double rand_double() {
	return static_cast<double>(rand()) / static_cast<double>(RAND_MAX);
}

void NNetwork::mutate (const Generation& parent) {	
	for (size_t i = 0; i < conns.size(); ++i) {
		if (conns[i].startid == c_startid && c_it == conns.size()) {
			c_it = i;
		}
			
		if (randomChance(CHANGE_WEIGHT_CHANCE)) {
			mutateChangeWeightValue(i);
		}
		
	}
	
	bool addnode = randomChance(ADD_NODE_CHANCE);
	if (addnode) {
		size_t n_connidx = rand() % conns.size();
	}
	
	bool addconn = randomChance(ADD_CONN_CHANCE);
	size_t c_startid = numNodes;
	if (addconn) {
	}
	size_t c_it = conns.size();
	
	//keep a list of nodes
	
	auto itstart = conns.begin();
	size_t count = 0;
	for (auto it=conns.begin(); it!=conns.end(); ++it) {//size_t i = 0; i < conns.size(); ++i) {
		if (itstart->startid != it->startid) {
			if (count < (number of nodes after this)) {
				pick an end node;
				insert(it, etc.)
			}
			itstart = it;
		}
		
		if (randomChance(CHANGE_WEIGHT_CHANCE)) {
			mutateChangeWeightValue(i);
		}
		
		count++;
	}
	if (addconn) {
		//pick a start id
		//find its range in conns
		//if range is full: repeat
		
		size_t c_startid, c_endid;
		auto itstart, itend;
		do {
			c_startid = randint(numOutputs+1, numNodes-1);	//pick a start id	

			auto it = conns.begin();								//find the lower range in conns
			while (it->startid != c_startid) { ++it; }
			itstart = it;
			
			size_t count = 0;										//find the upper range in conns
			while (it->startid == c_startid) { ++it; count++; }		//count the number of elements between lower and upper
			itend = it;
		while (count == numNodes-c_startid-1 + numOutputs);		//make sure range is not full of edges
		
		//look through [itstart, itend) for possible endids
		//
		
		// [0, numOutputs): endid = result
		// [numOutputs, ):  endid = result + numInputs
		do {
			c_endid = randint(0, numNodes-c_startid-1 + numOutputs);		//pick an end id
			if (c_endid >= numOutputs) { c_endid += numInputs; }
			
			auto it = itstart;										//look whether edge startid->endid is already created
			while (it != itend) {									//	(found in conns[itstart:itend])
				if (it->endid == c_endid) {							
					break;
				}
			}
		} while (it != itend);
		
		//add connection from c_startid to c_endid
		conns.insert(itend, Edge(c_startid, c_endid, parent.getInnov(c_startid, c_endid), 2 * (rand_double() - 0.5), true));
		
		
	}	
	if (randomChance(ADD_NODE_CHANCE)) {
		auto it = conns.begin()+(rand() % conns.size());
		Edge conn = *it;
		//mutateAddNode(i);
		size_t nid = addNode();
		
		disableConnection(i);
		addConnection(conn.startid, nid, parent.getInnov(conn.startid, nid), 1);
		addConnection(nid, conn.endid, parent.getInnov(nid, conn.endid), conn.weight);
		
		/*conns.insert(it, Edge(conn.startid, nid, parent.getInnov(conn.startid, nid), 1, true));
		while (it->startid == conn.startid) {
			++it;
		}
		
		conns.insert(it, Edge(nid, conn.endid, parent.getInnov(nid, conn.endid), conn.weight, true));*/
		//nodes[nid]->addConnection(nodes[conn.startid], 1);
		
		//conns.push_back(Edge(nid, conn.endid, parent.getInnov(nid, conn.endid), weight, true));
		//nodes[endid]->addConnection(nodes[startid], weight);
	}
	
	if (randomChance(ADD_CONN_CHANCE)) {
		for (unsigned int i=0; i<ADD_CONN_TRIES; i++) {
			//pick a random start node (not an output)
			//pick a random end node (not an input)
			//if it's a valid connection: make it
			size_t startid = randint(0, numNodes-numOutputs);
			if (startid >= numInputs) {
				startid += numOutputs;
			}
			size_t endid = randint(numOutputs+1, numNodes-1);
			
			//check that endid doesn't connect to startid
			if (!hasPath(endid, startid)) {
				addConnection(startid, endid, parent.getInnov(startid, endid), 2 * (rand_double() - 0.5));
				break;
			}
		}
	}
	
	//grouped by startid, in order of calculation

	
	precalc();

}

//returns whether there is
bool NNetwork::hasPath(size_t endid, size_t startid) {
	//can startid
	std::queue<NodeType*> queue;
	queue.push(nodes[endid]);
	while (!queue.empty()) {
		NodeType *curr = queue.front();
		if (curr == nodes[startid])
			return true;
		queue.pop();
		for (auto conn : curr->connections) {
			queue.push(conn->node);
		}
	}
	return false;
}

/*void NNetwork::mutateChangeWeightValue (Edge &conn) {
	double newValue;

	if (randomChance(RANDOM_WEIGHT_CHANCE)) {
		newValue = 2 * (rand_double() - 0.5);
	} else {

		double crnt = conns[index].weight;

		double maxAdd = std::min(1 - crnt, MAX_WEIGHT_DIFF);
		double maxSub = std::max(-1 + crnt, -MAX_WEIGHT_DIFF);

		double rand = rand_double();
		newValue = ((maxAdd - maxSub) * rand) + maxSub;
	}

	conn.weight = newValue;
}*/

void NNetwork::mutateChangeWeightValue (size_t index) {
	double newValue;

	if (randomChance(RANDOM_WEIGHT_CHANCE)) {
		newValue = 2 * (rand_double() - 0.5);
	} else {

		double crnt = conns[index].weight;

		double maxAdd = std::min(1 - crnt, MAX_WEIGHT_DIFF);
		double maxSub = std::max(-1 + crnt, -MAX_WEIGHT_DIFF);

		double rand = rand_double();
		newValue = ((maxAdd - maxSub) * rand) + maxSub;
	}

	conns[index].weight = newValue;
}