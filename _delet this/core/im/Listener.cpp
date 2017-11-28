//
// Created by, Jeremy S, 10/2/17
//

#include "Listener.hpp"

#include "Hub.hpp"

using namespace im;

Listener::Listener() {}

Listener::Listener(Channel const &chan) {
    this->subscribeTo(chan);
}

void Listener::subscribeTo(Channel const &chan) {
    Hub::instance().addSubscriber(chan, this);
}
