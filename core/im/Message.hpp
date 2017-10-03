//
// Created by Jeremy S, 9/27/17
//
// im::Message                  (object)
//      An object to represent a message that is passed between
//      objects using the internal messaging system.
//
//      Use lang::as<T>(msg[i]) to decode data from the message.
//      See Any.hpp for more information on the lang::Any type.

#ifndef IM_MESSAGE_HPP
#define IM_MESSAGE_HPP

#include <vector>
#include "../Any.hpp"

namespace im {

    struct Message {

        std::vector<lang::Any> contents;

        // --- Convenince Operator Overloads --- //
        lang::Any operator[] (size_t idx) const;

        lang::Any operator[] (int idx) const;

    };

}

#endif
