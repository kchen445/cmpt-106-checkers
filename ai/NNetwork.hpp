#pragma once

#include "../ml/sysml.hpp"

#include <memory>
#include <vector>
#include <forward_list>
#include <array>

namespace network {
	struct Edge {
		size_t startid;
		size_t endid;
		
		double weight;
	};

	template<size_t In, size_t Out>
	class NNetwork : public NetworkType<In, Out> {	
    public:
		//sigmoid function to be used in the network
		double sigmoid(double value);
		
		std::forward_list<std::vector<Edge>> conns;
		std::vector<double> biases;		//to-do: remove input nodes from biases
		
		size_t connsize;
		size_t numConns;
		size_t numNodes;
		
	/* saving, loading, constructors, destructors */
		NNetwork();
		
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
		void addConnection(std::forward_list<std::vector<Edge>>::iterator it, size_t startid, size_t endid, double weight);
		void addConnection(std::forward_list<std::vector<Edge>>::iterator it, size_t endid, double weight);

		//Add a node to the network and return its edge list
		std::forward_list<std::vector<Edge>>::iterator addNode(std::forward_list<std::vector<Edge>>::iterator it);
		std::forward_list<std::vector<Edge>>::iterator addNodeWithoutBias(std::forward_list<std::vector<Edge>>::iterator it);

	/* Calculation functions */
        std::array<double, Out> solve (std::array<double, In> const &input);
		
    /* Mutation functions */
        void tweakWeight (int chance, double range);
        void randomizeWeight (int chance);
		
		void mutateWeight(int chance, int bigchance, double range);
		void mutateBias(int chance, int bigchance, double range);
		
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
