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
#include <tuple>
#include "../core/im/Listener.hpp"

namespace network {

    class NNetworkOutputType : public im::Listener {
    private:

        size_t numberOfNodesLeftToReceive;

        // Recursively append more elements to outputValues until able
        // to add a value with a given id.
        void addValue (size_t id, double val) {
            if (id >= outputValues.size()) {
                outputValues.push_back(0);
                addValue(id, val);
            } else {
                outputValues.at(id) = val;
            }
        }

    protected:

        // Node ids are used as indices for the values.
        std::vector<double> outputValues;

    public:

        NNetworkOutputType ()
                : im::Listener(im::Channel::neuralOutputNode),
                  numberOfNodesLeftToReceive(0),
                  outputValues()
        {
            subscribeTo(im::Channel::neuralOutputNodeCreated);
        }

        // Once it gets a message from a node, store its id and value.
        void onMessageReceived (im::Channel const &chan, im::Message const &msg) {

            size_t id = 0;
            double value = 0;

            switch (chan) {
                // A new output node has been created, await for its output value.
                case im::Channel::neuralOutputNodeCreated: {
                    numberOfNodesLeftToReceive++;
                    addValue(lang::as<size_t>(msg[0]), 0);
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
                    }
                    break;
                }

                default: break;
            }

        }

        // Called once all the outputs have been received.
        virtual void onReceivedAllOutputs () = 0;

    };

}

#endif //AI_NNETWORKOUTPUTTYPE_HPP
