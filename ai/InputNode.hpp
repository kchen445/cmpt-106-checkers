//
// Created by Jeremy S on 2017-09-26.
//
// network::InputNode               (object)
//      A child class of network::NodeType<double> used
//      to represent an input node for the network.
//
//      Activation Function Type: bypass
//          Does not change the rawValue during activation.
//
//      Use the method fireWithValue(double) to trigger the
//      node with a given value.

#ifndef NNETWORK_INPUTNODE_HPP
#define NNETWORK_INPUTNODE_HPP

#include "NodeType.hpp"

namespace network {

    class InputNode : public NodeType<double> {
    public:

        // Default constructor
        InputNode ();

        // Bypass activation function.
        double activate ();

        // Sets rawValue to be a given value then calls send()
        void fireWithValue (double val);

        // 'I'
        char getType () const;

    };

}


#endif //NNETWORK_INPUTNODE_HPP
