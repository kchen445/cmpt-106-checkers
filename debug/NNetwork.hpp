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

#pragma once

#include <array>
#include <memory>
#include <string>

#include "../ml/sysml.hpp"

#include <string>

#include <vector>
#include <forward_list>
#include <array>

namespace network {
	struct Edge {
		size_t startid;
		size_t endid;
		size_t innov;
		
		double weight;
		bool enabled;
	};

	template<size_t In, size_t Out>
	class NNetwork : public NetworkType<In, Out> {	
    public:
		//sigmoid function to be used in the network
		double sigmoid(double value);
		
		std::forward_list<std::vector<Edge>> conns;
		std::vector<double> biases;		//to-do: remove input nodes from biases
		
		size_t connsize;
		size_t numNodes;
		
	/* saving, loading, constructors, destructors */
        //Construct a fully connected neural network with a number of nodes between each layer
		NNetwork(std::vector<size_t> layers);
		
		//Copy constructor
		NNetwork(const NNetwork &other);
        
		//Load a neural network from a file
        NNetwork(const std::string &filename);
        
		//Save the neural network to a file
        void save(const std::string &filename);
		
		//Destructor
        ~NNetwork();
		
		
        // Should return a network with the same configuration.
        // The returned network should be an isolated entity that
        // shares no pointers or anything with the base network.
        //
        // The network should be declared used the new operator.
		std::shared_ptr<NetworkType<In, Out>> clone();
		
		
	/* Network modification functions */		
		//Add a connection to the neural network
		//void addConnection(std::forward_list<std::vector<Edge>>::iterator it, size_t endid, double weight);

	/* Calculation functions */
        std::array<double, Out> solve (std::array<double, In> const &input);
		
    /* Mutation functions */
        void tweakWeight (int chance, double range);
        void randomizeWeight (int chance);

        void tweakBias (int chance, double range);
        void randomizeBias (int chance);
		
		void mutateNode(int chance);
		void mutateConnection(int chance, int retries=10);
	};

}

#include "NNetwork.cpp"

/*
// Compare networks based on fitness values.
inline bool operator< (network::NNetwork const & lhs, network::NNetwork const &rhs) {
    return lhs.fitnessValue < rhs.fitnessValue;
}
*/
