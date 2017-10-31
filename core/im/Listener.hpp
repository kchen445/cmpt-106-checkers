//
// Created by Jeremy S, 9/29/17
//
// im::Listener                 (abstract base class)
//      A base class to be extended by any class that wishes
//      to listen for messages from other objects.
//
//      Any child class MUST implement the abstract method:
//          void onMessageRecived(im::Message const &msg)
//      from im::foundation::ListenerType.
//
//      NOTE:
//      Listener objects should be alocated for the duration
//      of the program. This means, either defining them as
//      global variabls, in the main() function, or using 
//      the 'new' operator and not deleting the pointer.

#ifndef IM_LISTENER_HPP
#define IM_LISTENER_HPP

#include "ListenerType.hpp"
#include "Hub.hpp"

namespace im {

    class Listener : public foundation::ListenerType {
    public:

        Listener ();

        // Subscribes this object to a given channel upon initalization.
        Listener (Channel const &chan);

        // Subscribes this object to a given channel.
        void subscribeTo (Channel const &chan);

    };

}

#endif
