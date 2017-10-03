//
// Created by Jeremy S, 10/2/17
//

#include "Sender.hpp"

#include "Hub.hpp"

using namespace im;

Sender::Sender()
        :__channel(lang::nil)
{}

Sender::Sender(Channel const &channel)
        :__channel(channel)
{}

void Sender::sendMessage(Message const &msg) {
    if (__channel != lang::nil) {
        Hub::instance().sendMessageToSubscribers(__channel.value(), msg);
    } else {
        throw std::runtime_error("im::Sender::sendMessage(im::Message): No channel specified");
    }
}

void Sender::sendMessage(Channel const &chan, Message const &msg) {
    Hub::instance().sendMessageToSubscribers(chan, msg);
}
