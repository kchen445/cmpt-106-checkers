//
// Created by Jeremy S on 2017-09-29.
//
// --- FOR DEBUG --- //
// Prints the output of all the nodes to the screen

#ifndef AI_NNETWORKPRINTOUTPUT_HPP
#define AI_NNETWORKPRINTOUTPUT_HPP

#include "NNetworkOutputType.hpp"

namespace network {

    class NNetworkPrintOutput : public NNetworkOutputType {
    public:

        NNetworkPrintOutput ();

        // Print the output values of all nodes to the screen.
        void onReceivedAllOutputs ();

    };

}

#endif //AI_NNETWORKPRINTOUTPUT_HPP
