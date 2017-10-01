//
// Created by Jeremy S on 2017-09-30.
//

#include "ThresholdNode.hpp"

using namespace network;

ThresholdNode::ThresholdNode()
        : NodeType<double>(),
          threshold(0)
{}

ThresholdNode::ThresholdNode(double threshold)
        : NodeType<double>(),
          threshold(threshold)
{}

double ThresholdNode::activate() {
    return rawValue >= threshold ? 1 : -1;
}

char ThresholdNode::getType() const {
    return 'T';
}
