//	Neural Network class
//	The nodes in the network are stored in 3 vectors,
//		inputs		(for input nodes)
//		outputs		(for output nodes)
//		internals	(for hidden nodes)
//	
//	Nodes are assumed to be stored in topological order, or properly "linearized":
//	each node only depends on the values of nodes before it.
//	The input and output nodes are always ordered correctly,
//	but internal nodes may be moved out-of-order with neuron insertions.
//	You can use linearize() to reorder it.
//
//	Use calculate() to update the output node values (after linearizing)
//	You have to write input values/read output values from the inputs/outputs vectors directly (for now)
//
//	Files are saved in the following format, each token separated by whitespace:
// 		first line: node data
//			<size_t>:	number of nodes N
//			<char[]>:	N characters, listing the type of each node (I, T, or O)
//			ex. 12 IIIIITTTOOOO
//		following lines: connection data
//			<size_t>:	start node	(0 <= val < N)
//			<size_t>:	end node	(0 <= val < N)
//			<double>: 	weight of connection
//			ex. 5 9 0.7

#ifndef NNETWORK_NNETWORK_HPP
#define NNETWORK_NNETWORK_HPP

#include "NodeType.hpp"
#include "ThresholdNode.hpp"
#include "InputNode.hpp"
#include "OutputNode.hpp"
#include "NNetworkOutputType.hpp"
#include <vector>

namespace network {

    using Connection = ConnectionType<double, NodeType<double>>;

	class NNetwork {
    public:

        // Nodes
        std::vector<InputNode*> inputs;
        std::vector<NodeType<double>*> internals;
        std::vector<OutputNode*> outputs;

        // Output Device
        NNetworkOutputType* outputDevice;

        //Construct an empty neural network
        NNetwork(NNetworkOutputType* outputDevice);

        //Load a neural network from a file
        NNetwork(NNetworkOutputType* outputDevice, const char *filename);

        ~NNetwork();

        //Linearizes [internals] for calculation
        //	[haven't really tested this yet, hope it works]
        void linearize();

        //Propagate the input values through the network, assuming linearized
        void calculate(std::vector<double> const &inputValues);

        //Save the neural network to a file
        void save(const char *filename);
	};

}
#endif