//
// Created by Jeremy S on 2017-09-30.
//

#include "ThresholdNode.hpp"

using namespace network;

ThresholdNode::ThresholdNode()
        : NodeTypeEx<double>(),
          threshold(0)
{}

ThresholdNode::ThresholdNode(double threshold)
        : NodeTypeEx<double>(),
          threshold(threshold)
{}

double ThresholdNode::activationFunction(double const &in) {
    return in >= threshold ? 1 : -1;
}

char ThresholdNode::getType() const {
    return 'T';
}
