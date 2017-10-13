//
// Created by Jeremy S on 2017-09-30.
//

#include "ThresholdNode.hpp"

using namespace network;

ThresholdNode::ThresholdNode()
        : NodeType('T'),
          threshold(0)
{}

ThresholdNode::ThresholdNode(double threshold)
        : NodeType('T'),
          threshold(threshold)
{}

double ThresholdNode::activationFunction(double in) {
    return in >= threshold ? 1 : -1;
}
