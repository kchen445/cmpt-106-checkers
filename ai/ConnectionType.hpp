//
// Created by Jeremy S on 2017-09-26.
//
// network::ConnectionType<T, Node>         (object)
//      An object to represent the connection between 2 nodes.
//      Each connection has a weight value associated with it
//      which is multiplied with all values that pass through
//      the connection.
//
//      Example:
//          0.5 IN ---> Connection [weight = 2] ---> 1.0 OUT

#ifndef NNETWORK_CONNECTIONTYPE_HPP
#define NNETWORK_CONNECTIONTYPE_HPP

namespace network {

    // Generic connection object.
    // Template Restrictions:
    //      T must have valid operator += and *
    //      Node should be network::NodeType<T>,
    //          where T is the same T in this template
    //
    // Note: The 'Node' template is used so that this
    //       header file does not have to include
    //       NodeType.hpp as doing so creates a feedback
    //       loop of #includes.
    template<typename T, typename Node>
    struct ConnectionType {

        // Destination node for this connection.
        Node& node;

        // Weight value for this connection.
        T weight;

        // Constructor
        ConnectionType (Node& node, T const &weight)
                : weight(weight), node(node)
        {}

        // Transfers data through this connection.
        //
        // The input value, 'value', is multiplied by the
        // internal weight value and then added to the
        // rawValue of the destination node.
        //
        // If the destination node does not require any
        // more input values then the destination node's
        // send method is called.
        void feedForward (T const &value) {
            node.rawValue += (weight * value);
            node.numInputs--;
            if (node.numInputs == 0) {
                node.send();
            }
        }

    };

}



#endif //NNETWORK_CONNECTIONTYPE_HPP
