//
// Created by Jeremy S on 2017-09-30.
//

#include"InputNode.hpp"

using namespace network;

InputNode::InputNode(double const &value)
        : NodeType('I', value)
{}
		
InputNode::InputNode()
        : NodeType('I')
{}

double InputNode::activationFunction(double in) {
    return in;
}

double InputNode::calculate() {
    return rawValue;
}
