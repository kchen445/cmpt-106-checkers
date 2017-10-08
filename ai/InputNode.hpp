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

#include "NodeTypeEx.hpp"

namespace network {

    class InputNode : public NodeTypeEx<double> {
    public:

        // Default constructor
        InputNode(double const &value);
		InputNode();

        // Bypass activation function.
        double activationFunction (double const &in);

        double calculate ();

        // 'I'
        char getType () const;

    };

}


#endif //NNETWORK_INPUTNODE_HPP
