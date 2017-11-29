//
// Created by Jeremy Schwartz, 10/13/17
//
// network::Connection<Node>                (object)
//  An object to represent a connection between two nodes.
//  The base node for the connection is the node which
//  containes this connection object. The target node is
//  a dependency of the base node.
//
//      Target Node -> Connection -> Base Node
//
//  This object is templated so that it does not need to
//  include NodeType.hpp as doing so creates a macro loop.

#ifndef NNETWORK_CONNECTIONTYPE_HPP
#define NNETWORK_CONNECTIONTYPE_HPP

namespace network {

    template<typename Node>
    struct Connection {

        // A pointer to the target node.
        Node* node;

        // The weight value for this connection.
        double weight;

        // Constructors
        Connection (Node* fromNode, double const &weight) 
                : node(fromNode), weight(weight)
        {}

        Connection ()
                : node(nullptr), weight(0)
        {}

        Connection (Connection<Node> const &other)
                : node(other.node), weight(other.weight)
        {}

        // Returns the output value of the target node
        // multiplied by the weight value.
        //
        // This method is half of the recursive algorithm
        // that solves the network with NodeType::calculate
        // being the other half.
        double getValue () {
            return weight * node->calculate();
        }

    };

}

#endif
