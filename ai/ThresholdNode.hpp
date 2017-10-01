//
// Created by Jeremy S on 2017-09-26.
//
// network::ThresholdNode           (object)
//      A child class of network::NodeType<double> used
//      to represent a neuron with a simple binary
//      activation function.
//
//      Activation Function Type: threshold
//          If rawValue is >= to the threshold then fires
//          as 1 else fires as -1.

#ifndef NNETWORK_THRESHOLDNODE_HPP
#define NNETWORK_THRESHOLDNODE_HPP

#include "NodeType.hpp"

namespace network {

    class ThresholdNode : public NodeType<double> {
    public:

        double threshold;

        // Default constructor, threshold = 0
        ThresholdNode ();

        // Threshold constructor
        ThresholdNode (double threshold);

        // Activation function as described above.
        double activate () override;

        char getType () const override;

    };

}


#endif //NNETWORK_THRESHOLDNODE_HPP
