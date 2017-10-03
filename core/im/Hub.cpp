//
// Created by, Jeremy S, 10/2/17
//

#include "Hub.hpp"

// Static variable definition
im::Hub* im::Hub::__instance = nullptr;

using namespace im;

Hub& Hub::instance() {
    if (__instance == nullptr) {
        __instance = new Hub{};
    }
    return *__instance;
}

Hub::Hub()
        : subscribers()
{}

void Hub::addSubscriber(Channel const &chan, foundation::ListenerType* lptr) {
    try {
        subscribers.at(chan).push_back(lptr);
    } catch (...) {
        subscribers.insert({chan, std::vector<foundation::ListenerType*>{}});
        subscribers.at(chan).push_back(lptr);
    }
}

void Hub::sendMessageToSubscribers(Channel const &chan, Message const &msg) {
    // map::at() will throw an error if there is no key with a given value.
    // This means that there are no listeners for that channel so do nothing.
    try {

        for (size_t i = 0; i < subscribers.at(chan).size(); ++i) {
            subscribers.at(chan).at(i)->onMessageReceived(chan, msg);
        } 

    } catch (...) { return; }
}
