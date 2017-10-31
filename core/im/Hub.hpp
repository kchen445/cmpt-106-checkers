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

        // Instance interface method.
        // Uses lazy initalization of the instance.
        static Hub& instance ();

        Hub ();

        // Subscribes an object to a given channel.
        void addSubscriber (Channel const &chan, foundation::ListenerType* lptr);

        // Sends a message to all subscribed objects of a given channel.
        void sendMessageToSubscribers (Channel const &chan, Message const &msg);

    };

}

#endif
