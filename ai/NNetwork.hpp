//	Neural Network class
//	The nodes in the network are stored in 3 vectors,
//		inputs		(for input nodes)
//		outputs		(for output nodes)
//		internals	(for hidden nodes)
//	node ids are counted from 0, in the order of vectors above:
//	ex. inputs go 0,1,2,3,4, then outputs 5,6,7, then internals 8,9,10
//	Connections are also stored in a vector,
//		conns
//
//	Currently, connections are double-stored,
//		once in conns (for storage),
//		once as an actual link between nodes (for calculation)
//	because of this, please use the connection functions to modify the network!
//
//	Use calculate(inpvalues) to calculate outputs from inputs
//
//	Files are saved in the following format, each token separated by whitespace:
// 		first line: node data
//			<size_t>:	number of nodes N
//			<size_t>:	number of input nodes
//			<size_t>:	number of output nodes
//			<size_t>:	number of internal nodes
//			ex. 12 5 4 3
//		following lines: connection data
//			<size_t>:	end node	(0 <= val < N)
//			<size_t>:	start node	(0 <= val < end node)
//			<double>: 	weight of connection
//			<uint>: 	innovation number
//			<bool>:		is connection enabled?
//			ex. 9 5 0.7 4 0

#ifndef NNETWORK_NNETWORK_HPP
#define NNETWORK_NNETWORK_HPP

#include "NodeType.hpp"
#include "ThresholdNode.hpp"
#include "InputNode.hpp"
#include "OutputNode.hpp"
#include "NNetworkOutputType.hpp"
#include <vector>

namespace network {
	
	struct Edge {
		size_t endid;
		size_t startid;
		double weight;
		
		size_t innov;
		bool enabled;
	};
	
    //using Connection = ConnectionType<double, NodeTypeEx<double>>;

	class NNetwork {
	private:
		//fetch a node from within the vectors given its id
		NodeType* getNode(size_t id);
	
    public:
        // Nodes
        std::vector<InputNode*> inputs;
        std::vector<OutputNode*> outputs;
		std::vector<NodeType*> internals;
		size_t numNodes;
		
		std::vector<Edge> conns;
		
        // Output Device
        NNetworkOutputType* outputDevice;

        //Construct an empty neural network
        NNetwork(NNetworkOutputType* outputDevice);

        //Load a neural network from a file
        NNetwork(NNetworkOutputType* outputDevice, const char *filename);

        ~NNetwork();
		
		//Add a (hidden) node to the neural network and returns its id
		size_t addNode();
		//size_t addNode(connections)?
		
		//deleting nodes is pretty hairy cause you have to repair the connections -- not gonna get into that rn
		
		//Add a connection to the neural network
		void addConnection(size_t endid, size_t startid, double weight, unsigned int innov, bool enabled = true);
		
		//Enable a connection, given its index within the [conns] list
		void enableConnection(size_t idx);
		//Disable a connection, given its index within the [conns] list
		void disableConnection(size_t idx);

        //Return the value of the output nodes, given some inputs
        std::vector<double> calculate(std::vector<double> const &inputValues);

        //Save the neural network to a file
        void save(const char *filename);

        // Returns a value which coresonds the similarity between two networks.
        // Larger values mean a larger difference and so on.
        //
        //  d = (c1 * E)/N + (c2 * D)/N + (c3 * W)
        //
        // Formula from K. O. Stanley and R. Miikkulainen Neural Evolution
        // through Augmenting Topologies.
        double difference (NNetwork& other);
	};

}
#endif
