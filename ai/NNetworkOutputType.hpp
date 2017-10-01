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

        // Recursively append more elements to outputValues
        // until able to add a value with a given id.
        void addValue (size_t id, double val);

    protected:

        // Node ids are used as indices for the values.
        std::vector<double> outputValues;

    public:

        NNetworkOutputType ();

        // Once it gets a message from a node, store its id and value.
        void onMessageReceived (im::Channel const &chan, im::Message const &msg);

        // Called once all the outputs have been received.
        virtual void onReceivedAllOutputs () = 0;

    };

}

#endif //AI_NNETWORKOUTPUTTYPE_HPP
