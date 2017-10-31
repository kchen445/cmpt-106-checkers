//
// Created by Jeremy S on 2017-09-29.
//

#include "OutputNode.hpp"
#include <cmath>

using namespace network;

size_t OutputNode::nextId = 0;

OutputNode::OutputNode()
        : NodeType('O'),
          im::Sender(im::Channel::neuralOutputNode),
          id(OutputNode::nextId++)
{
    //sendMessage(im::Channel::neuralOutputNodeCreated, {{id}});
}

double OutputNode::activationFunction(double in) {
    return tanh(in/2);
    //return in >= 0 ? 1 : -1;
}

double OutputNode::calculate() {

    double calculatedValue = 0;
    for (auto conn : connections) {
        calculatedValue += conn.getValue();
    }

    rawValue = activationFunction(calculatedValue);

    //sendMessage({{id, rawValue}});
    return rawValue;
}
