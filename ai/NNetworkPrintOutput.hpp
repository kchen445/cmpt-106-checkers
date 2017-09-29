//
// Created by Jeremy S on 2017-09-29.
//
// --- FOR DEBUG --- //
// Prints the output of all the nodes to the screen

#ifndef AI_NNETWORKPRINTOUTPUT_HPP
#define AI_NNETWORKPRINTOUTPUT_HPP

#include <iostream>
#include "NNetworkOutputType.hpp"

namespace network {

    class NNetworkPrintOutput : public NNetworkOutputType {
    public:

        NNetworkPrintOutput (size_t numberOfNodes)
                : NNetworkOutputType(numberOfNodes)
        {}

        void onReceivedAllOutputs () {
            for (size_t i = 0; i < outputValues.size(); ++i) {
                std::cout << "Id[" << i << "]: " << outputValues[i] << std::endl;
            }
        }

    };

}

#endif //AI_NNETWORKPRINTOUTPUT_HPP
