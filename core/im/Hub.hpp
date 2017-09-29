//
// Created by, Jeremy S, 9/29/17
//
// im::im::Hub              (singleton object)
//      This object handles the sending of messages between objects
//      and should not be interfaced with directly. Instead, objects
//      should extend im::Listener or im::Sender.

#ifndef IM_HUB_HPP
#define IM_HUB_HPP

#include <map>
#include <vector>
#include "Message.hpp"
#include "Channel.hpp"
#include "ListenerType.hpp"

namespace im {

    class Hub {
    private:

        static Hub* __instance;

        std::map<Channel, std::vector<foundation::ListenerType*>> subscribers;

    public:

        static Hub& instance () {
            if (__instance == nullptr) {
                __instance = new Hub{};
            }
            return *__instance;
        }

        Hub ()
        : subscribers()
        {}

        void addSubscriber (Channel const &chan, foundation::ListenerType* lptr) {
            try {
                subscribers.at(chan).push_back(lptr);
            } catch (...) {
                subscribers.insert({chan, std::vector<foundation::ListenerType*>{}});
                subscribers.at(chan).push_back(lptr);
            }
        }

        void sendMessageToSubscribers (Channel const &chan, Message const &msg) {
            try {

                for (size_t i = 0; i < subscribers.at(chan).size(); ++i) {
                    subscribers.at(chan).at(i)->onMessageReceived(msg);
                } 

            } catch (...) { return; }
        }

    };

}

im::Hub* im::Hub::__instance = nullptr;

#endif
