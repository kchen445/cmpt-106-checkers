#include <iostream>
#include "Listener.hpp"
#include "Sender.hpp"

using std::cout;
using std::endl;


struct ObjS : public im::Sender {

    ObjS (im::Channel const &chan)
    : im::Sender(chan)
    {}

};


struct ObjL : public im::Listener {

    ObjL ()
    : im::Listener()
    {
        this->subscribeTo(im::Channel::testChan1);
    }

    void onMessageReceived (im::Channel const &chan, im::Message const &msg) {
        cout << "Message from 'testChan1' recieved" << endl;
        cout << "[0] " << lang::as<int>(msg[0]) << endl; 
        cout << "[1] " << lang::as<char>(msg[1]) << endl;
    }

};

struct ObjA : public im::Listener {

    ObjA ()
    : im::Listener()
    {
        this->subscribeTo(im::Channel::testChan2);
    }

    void onMessageReceived (im::Channel const &chan, im::Message const &msg) {
        cout << "Message from 'testChan2' recieved" << endl;
        cout << "[0] " << lang::as<std::string>(msg[0]) << endl; 
    }

};


int main () {

    ObjS s{im::Channel::testChan1};
    ObjS t{im::Channel::testChan2};
    ObjL l;
    ObjA a;

    s.sendMessage({{15, 'a'}});
    t.sendMessage({{std::string("Hello")}});

}
