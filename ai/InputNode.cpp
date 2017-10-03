//
// Created by Jeremy S on 2017-09-30.
//

#include"InputNode.hpp"

using namespace network;

InputNode::InputNode(double const &value)
        : NodeTypeEx<double>(value)
{}

double InputNode::activationFunction(double const &in) {
    return in;
}

double InputNode::calculate() {
    return rawValue;
}

char InputNode::getType() const {
    return 'I';
}
