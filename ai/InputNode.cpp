//
// Created by Jeremy S on 2017-09-30.
//

#include"InputNode.hpp"

using namespace network;

InputNode::InputNode()
        : NodeType<double>()
{}

double InputNode::activate() {
    return rawValue;
}

void InputNode::fireWithValue (double val) {
    rawValue = val;
    send();
}

char InputNode::getType() const {
    return 'I';
}
