// NOTE: This is a temp test file //

#include "NNetwork.hpp"
#include "NNetworkPrintOutput.hpp"

im::Hub* im::Hub::__instance = nullptr; // To be added to its own src file at a later date.

using namespace network;

int main () {

    NNetwork neural{new NNetworkPrintOutput{}, "neural.txt"};
    neural.linearize();
    neural.calculate({-1, 8, -1});

}