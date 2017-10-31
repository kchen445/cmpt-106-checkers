//
// Created by Jeremy S, 10/2/17
//

#include "Message.hpp"

using namespace im;

lang::Any Message::operator[] (size_t idx) const {
    return contents.at(idx);
}

lang::Any Message::operator[] (int idx) const {
    return contents.at(idx);
}