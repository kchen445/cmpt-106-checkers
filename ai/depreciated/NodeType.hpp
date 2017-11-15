//
// Created by Jeremy S on 2017-09-26.
//
// network::NodeType<T>             (abstract base class)
//      The base class for all node/neuron objects.
//      All other node types should extend this class
//      and provide their own activate() method.
//
//      Child objects may also override the send() method
//      to change how the neuron is fired. An example of
//      this is an output node which prints its value to
//      the screen instead of sending it on.

#ifndef NNETWORK_NODE_HPP
#define NNETWORK_NODE_HPP

#include <stdlib.h>
#include <vector>
#include "ConnectionType.hpp"

namespace network {

    // Generic abstract class to represent a neuron in the neural network.
    // Template Restrictions:
    //      T must be constructable from 0
    //      T must have valid operators + and *
    template<typename T>
    class NodeType {
    public:

        // The number of connections into this node.
        size_t numInputs;

        // The 'raw', unactivated value for this node.
        T rawValue;

        // All the connections to other nodes.
        std::vector<ConnectionType<T, NodeType<T>>> connections;

        // Default constructor.
        NodeType ()
                : numInputs(0), rawValue(0), connections()
        {}

        // Copy constructor.
        NodeType (NodeType<T> const &other)
                : numInputs(other.numInputs),
                  rawValue(other.rawValue),
                  connections(other.connections)
        {}

        // Virtual Destructor (Does nothing)
        virtual ~NodeType () {}

        // Builds a connection between this node and another.
        void addConnection (NodeType<T>& node, T const &weight) {
            connections.push_back({node, weight});
            node.numInputs++;
        }

        // Abstract activation function.
        //      Should return a value between -1 and 1 based
        //      on the current rawValue.
        //
        // Child class MUST properly implement this method.
        virtual T activate () = 0;

        // "Fires this neuron"
        // First activates this node by calling activate(),
        // then send the result too all connected nodes via
        // the ConnectionType objects int the connections array.
        //
        // Child classes may override this method to do something
        // different when the node is triggered.
        virtual void send () {
            T value = activate();
            for (auto connection : connections) {
                connection.feedForward(value);
            }
        }

        // Returns the type of this node, used in saving to a file.
        virtual char getType () const = 0;

    };

}


#endif //NNETWORK_NODE_HPP
