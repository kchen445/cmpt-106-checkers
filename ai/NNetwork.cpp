#include "NNetwork.hpp"

#include <unordered_map>
#include <fstream>

using namespace network;

NNetwork::NNetwork(NNetworkOutputType* outputDevice)
        : inputs(),
          internals(),
          outputs(),
          outputDevice(outputDevice)
{}

NNetwork::NNetwork(NNetworkOutputType* outputDevice, const char *filename)
        : inputs(),
          internals(),
          outputs(),
          outputDevice(outputDevice)
{
	std::ifstream file(filename, std::ifstream::in);
	
	// --- parse first line: "12 IIIIITTTOOOO" ---
	
	//read number of nodes
	size_t num;
	file >> num >> std::ws;		//(skip whitespace after)
	
	//create nodes from node types
	std::vector<NodeType<double>*> nodes(num);	//temp list of nodes, to convert from id -> Node*
	for (size_t i=0; i<num; i++) {
		char type;								//read type
		file.get(type);
		switch (type) {
			case 'I':							//input node
				inputs.push_back(new InputNode());
				nodes[i] = inputs.back();
				break;
			case 'T':							//threshold node
				internals.push_back(new ThresholdNode());
				nodes[i] = internals.back();
				break;
			case 'O':							//output node
				outputs.push_back(new OutputNode());
				nodes[i] = outputs.back();
				break;
            default:
                throw std::runtime_error("parse error: invalid symbol detected");
		}
	}
	
	// --- parse connections: "5 9 0.7" ---
	while (file.good()) {
		size_t startid, endid;
		double weight;
		file >> startid >> endid >> weight >> std::ws;	//skip trailing whitespace too
		
		nodes[startid]->addConnection(*nodes[endid], weight);
	}
	file.close();
}
		
NNetwork::~NNetwork() {
	//delete all Node*s
	while(!inputs.empty()) delete inputs.back(), inputs.pop_back();
	while(!internals.empty()) delete internals.back(), internals.pop_back();
	while(!outputs.empty()) delete outputs.back(), outputs.pop_back();
}


//used with linearize()
//Visits all children of [node] and adds any non-output nodes to [internals]
void visit(NodeType<double>* node, std::unordered_map<NodeType<double>*, bool>& visited, std::vector<NodeType<double>*>& internals) {
	for (auto conn : node->connections) {
		if (!visited.count(&conn.node)) {
			visit(&conn.node, visited, internals);
		}
	}
	visited[node] = true;
	if (node->connections.size() > 0)
		internals.push_back(node);
}

void NNetwork::linearize() {
	//hashmap to check if Node* has been visited
	std::unordered_map<NodeType<double>*, bool> visited;
	
	//perform visiting of input nodes manually, without adding them to internals
	internals.clear();
	for (InputNode* nptr : inputs) {
		for (auto conn : nptr->connections) {
			if (!visited.count(&conn.node))
				visit(&conn.node, visited, internals);
		}
	}
}
		
void NNetwork::calculate(std::vector<double> const &inputValues) {
    if (inputValues.size() != inputs.size()) {
        throw std::runtime_error("Input value vector does not match number of input nodes.");
    }

	//since linearized, just run through them in order
	for (size_t i = 0; i < inputs.size(); ++i) {
        inputs[i]->fireWithValue(inputValues[i]);
	}
}
		
void NNetwork::save(const char *filename) {
	std::ofstream file(filename, std::ifstream::out);
	
	// --- fill in helper data structures ---
	std::unordered_map<NodeType<double>*,size_t> indices;	//Node* -> id
	std::vector<NodeType<double>*> nodes;					//id -> Node*
	size_t id = 0;
	for (auto node : inputs)	{ indices[node] = id++; nodes.push_back(node); }
	for (auto node : internals)	{ indices[node] = id++; nodes.push_back(node); }
	for (auto node : outputs)	{ indices[node] = id++; nodes.push_back(node); }
	
	// --- write first line ---
	//write number of nodes in network
	file << nodes.size() << ' ';
	
	//write out types of nodes in network
	for (auto node : nodes)
		file << node->getType();
	file << std::endl;
	
	// --- write connections ---
	for (size_t i=0; i<nodes.size(); i++) {
		for (auto conn : nodes[i]->connections) {
			NodeType<double>* next = &conn.node;
			file << i << ' '
				 << indices[next] << ' '
				 << conn.weight << std::endl;
		}
	}
	
	file.close();
}