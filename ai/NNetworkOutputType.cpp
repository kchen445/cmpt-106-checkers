//
// Created by Jeremy S on 2017-09-30.
//

#include "NNetworkOutputType.hpp"

using namespace network;

NNetworkOutputType::NNetworkOutputType()
        : im::Listener(),
          numberOfNodesLeftToReceive(0),
          totalNumberOfNodes(0),
          outputValues()
{
    subscribeTo(im::Channel::neuralOutputNode);
    subscribeTo(im::Channel::neuralOutputNodeCreated);
}

void NNetworkOutputType::addValue(size_t id, double val) {
    if (id >= outputValues.size()) {
        outputValues.push_back(0);
        addValue(id, val);
    } else {
        outputValues.at(id) = val;
    }
}

void NNetworkOutputType::onMessageReceived(im::Channel const &chan, im::Message const &msg) {
    size_t id = 0;
    double value = 0;

    switch (chan) {

        // A new output node has been created, await for its output value.
        case im::Channel::neuralOutputNodeCreated: {
            id = lang::as<size_t>(msg[0]);
            numberOfNodesLeftToReceive++;
            totalNumberOfNodes++;
            addValue(id, 0);
            break;
        }

        // An output node has a value, store it.
        case im::Channel::neuralOutputNode: {
            id = lang::as<size_t>(msg[0]);
            value = lang::as<double>(msg[1]);
            outputValues.at(id) = value;
            numberOfNodesLeftToReceive--;

            // Check if that was the last message that it was waiting for.
            if (numberOfNodesLeftToReceive == 0) {
                onReceivedAllOutputs();
                numberOfNodesLeftToReceive = totalNumberOfNodes;
            }
            break;
        }

        default: break;
    }
}
