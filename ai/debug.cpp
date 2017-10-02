// NOTE: This is a temp test file //

#include "NNetwork.hpp"
#include "NNetworkPrintOutput.hpp"

using namespace network;

int main () {

    // neural.txt must be located in the calling directory
    NNetwork neural{new NNetworkPrintOutput{}, "neural.txt"};
    neural.linearize();
    neural.calculate({-1, 8, -1});

}