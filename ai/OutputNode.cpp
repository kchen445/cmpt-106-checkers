//
// Created by Jeremy S on 2017-09-29.
//

#include "OutputNode.hpp"

using namespace network;

size_t OutputNode::nextId = 0;

OutputNode::OutputNode()
        : NodeType<double>(),
          im::Sender(im::Channel::neuralOutputNode),
          id(OutputNode::nextId++)
{
    sendMessage(im::Channel::neuralOutputNodeCreated, {{id}});
}

double OutputNode::activate() {
    return rawValue;
}

void OutputNode::send() {
    sendMessage({{id, rawValue}});
}

char OutputNode::getType () const {
    return 'O';
}
