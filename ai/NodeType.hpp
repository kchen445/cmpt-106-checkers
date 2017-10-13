//
// Created by Jeremy Schwartz, 10/13/17
//
// network::NodeType                (abstract base class)
//  The base class for all node/neuron objects.

#ifndef NNETWORK_NODETYPEEX_HPP
#define NNETWORK_NODETYPEEX_HPP

#include <vector>
#include "Connection.hpp"

namespace network {
    
    class NodeType {
    private:

        // The type of this node as a character.
        char type;

    public:

        // Stored value of the node. Output values are summed
        // up here before being passed though the activation
        // function and sent to the next node.
        double rawValue;

        // A list of connections to other nodes in the network.
        // The connections represent prerequisite nodes that
        // feed into this node.
        std::vector<Connection<NodeType>> connections;

        // Constructors
        NodeType (char type);

        NodeType (char type, double const &rawValue);

        // Destructor (does nothing)
        virtual ~NodeType () {}

        // Returns the value from this node by summing up the
        // value of the nodes connected to it and passing that
        // value through the activation function.
        //
        // Sets rawValue to be the output value once it has
        // been caluclated.
        //
        // This method is half of the recursive algorithm that
        // solves the network with Connection<Node>::getValue
        // being the other half.
        // The base case for the recursive structure is defined
        // in InputNode's override of this method.
        virtual double calculate ();

        // Abstract activation function to be implemented by 
        // subclasses.
        //
        // Return value MUST be between 1 and -1 or equal to 1 or -1.
        virtual double activationFunction (double calculatedValue) = 0;

        // Creates a connection between this node and another
        // with a given weight value.
        void addConnection (NodeType* node, double const &weight);

        // Getter function for type variable.
        char getType () const;

    };

}

#endif
