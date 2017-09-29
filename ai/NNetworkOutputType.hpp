//
// Created by Jeremy S on 2017-09-29.
//
// network::NNetworkOutputType              (abstract base class)
//      An abstract object to handle the output from the network
//      nodes. Base classes will define what happens once all the
//      output nodes have been fired.

#ifndef AI_NNETWORKOUTPUTTYPE_HPP
#define AI_NNETWORKOUTPUTTYPE_HPP

#include <vector>
#include "../core/im/Listener.hpp"

namespace network {

    class NNetworkOutputType : public im::Listener {
    private:

        size_t numberOfNodesLeftToReceive;

    protected:

        // Node ids are used as indices for the values.
        std::vector<double> outputValues;

    public:

        NNetworkOutputType (size_t numberOfOutputNodes)
                : im::Listener(im::Channel::neuralOutputNode),
                  numberOfNodesLeftToReceive(numberOfOutputNodes),
                  outputValues(numberOfOutputNodes)
        {}

        // Once it gets a message from a node, store its id and value.
        void onMessageReceived (im::Message const &msg) {
            size_t id = lang::as<size_t>(msg[0]);
            double value = lang::as<double>(msg[1]);
            outputValues.at(id) = value;
            numberOfNodesLeftToReceive--;

            // Check if that was the last message that it was waiting for.
            if (numberOfNodesLeftToReceive == 0) {
                onReceivedAllOutputs();
            }
        }

        // Called once all the outputs have been received.
        virtual void onReceivedAllOutputs () = 0;

    };

}

#endif //AI_NNETWORKOUTPUTTYPE_HPP
