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
#include "NNetworkOutputType.hpp"
#include <vector>
#include <string>

namespace network {
	class Generation;	//forward declaration to resolve circular reference
	
	struct Edge {
		size_t startid;
		size_t endid;
		size_t innov;
		
		double weight;
		bool enabled;
	};

	class NNetwork {	
    public:
        // Nodes
		std::vector<NodeType*> nodes;
		
		size_t numInputs;
		size_t numOutputs;
		size_t numNodes;
		
		std::vector<Edge> conns;
		
        // Output Device
        NNetworkOutputType* outputDevice;
		
	/* saving, loading, constructors, destructors */
        //Construct an empty neural network
        NNetwork(NNetworkOutputType* outputDevice, size_t numInputs, size_t numOutputs, size_t numHidden = 0);
		//Copy constructor
		NNetwork(const NNetwork &other);

        //Load a neural network from a file
        NNetwork(NNetworkOutputType* outputDevice, const std::string &filename);
        //Save the neural network to a file
        void save(const std::string &filename);
		
		//Destructor
        ~NNetwork();
		
	/* Network modification functions */
		//Add a (hidden) node to the neural network and returns its id
		size_t addNode();
		//Add a connection to the neural network
		void addConnection(size_t startid, size_t endid, size_t innov, double weight, bool enabled = true);
		
		//Enable a connection, given its index within the [conns] list
		void enableConnection(size_t idx);
		//Disable a connection, given its index within the [conns] list (note -- not very efficient)
		void disableConnection(size_t idx);

	/* Calculation functions */
        //Given some inputs, return the value of the output nodes
        std::vector<double> calculate(std::vector<double> const &inputValues);

	/* Difference functions */
        // Returns a value which coresonds the similarity between two networks.
        // Larger values mean a larger difference and so on.
        //
        //  d = (c1 * E)/N + (c2 * D)/N + (c3 * W)
        //
        // Formula from K. O. Stanley and R. Miikkulainen Neural Evolution
        // through Augmenting Topologies.
        double difference (const NNetwork &other);

    /* Mutation functions */
        void mutate(const Generation &parent);

        void mutateChangeWeightValue (size_t index);

        void mutateAddNode (size_t index);

        void mutateAddConn ();
	};

}
#endif
