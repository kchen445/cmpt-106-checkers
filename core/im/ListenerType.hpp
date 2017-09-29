//
// Created by Jeremy S, 9/ 29/17
//
// im::LiestenerType                (abstract base class)
//      Used in im::Hub to refer to Listener objects. Should NOT
//      be used anywhere else. Use im::Listener instead.

#ifndef IM_LISTENERTYPE_HPP
#define IM_LISTENERTYPE_HPP

#include "Channel.hpp"
#include "Message.hpp"

namespace im {

    namespace foundation {

        class ListenerType {
        public:

            virtual void subscribeTo (Channel const &chan) = 0;

            virtual void onMessageReceived (Message const &msg) = 0;

        };

    }
}

#endif
