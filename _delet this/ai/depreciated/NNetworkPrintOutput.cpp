//
// Created by Jeremy S on 2017-09-30.
//

#include "NNetworkPrintOutput.hpp"
#include <iostream>

using namespace network;

NNetworkPrintOutput::NNetworkPrintOutput()
        : NNetworkOutputType()
{}

void NNetworkPrintOutput::onReceivedAllOutputs() {
    for (size_t i = 0; i < outputValues.size(); ++i) {
        std::cout << "Id[" << i << "]: " << outputValues[i] << std::endl;
    }
}
