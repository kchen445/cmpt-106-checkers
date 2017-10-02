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
#include "../Optional.hpp"

namespace im {

    class Sender {
    private:

        lang::Optional<Channel> __channel;

    public:

        Sender ();

        Sender (Channel const &channel);

        void sendMessage (Message const &msg);

        void sendMessage (Channel const &chan, Message const &msg);

    };

}

#endif
