//
// Created by Jeremy S, 9/29/17
//
// im::Sender                   (base class)
//      A base class to be exended by objects which wish
//      to send messages to other objects.

#ifndef IM_SENDER_HPP
#define IM_SENDER_HPP

#include "Channel.hpp"
#include "Message.hpp"
#include "Hub.hpp"
#include "../Optional.hpp"

namespace im {

    class Sender {
    private:

        lang::Optional<Channel> __channel;

    public:

        Sender ()
        : __channel(lang::nil)
        {}

        Sender (Channel const &channel)
        : __channel(channel)
        {}

        void sendMessage (Message const &msg) {
            if (__channel != lang::nil) {
                Hub::instance().sendMessageToSubscribers(__channel.value(), msg);
            } else {
                throw std::runtime_error("im::Sender::sendMessage(im::Message): No channel specified");
            }
        }

        void sendMessage (Channel const &chan, Message const &msg) {
            Hub::instance().sendMessageToSubscribers(chan, msg);
        }

    };

}

#endif
