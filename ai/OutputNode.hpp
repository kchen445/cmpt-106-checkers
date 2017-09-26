//
// Created by Jeremy S on 2017-09-26.
//
// ---- FOR TESTING ONLY ---- //

#ifndef NNETWORK_OUTPUTNODE_HPP
#define NNETWORK_OUTPUTNODE_HPP

#include "NodeType.hpp"
#include <iostream>

namespace network {

    class OutputNode : public NodeType<double> {
    private:

        static int nextId;
        int id;

    public:

        OutputNode()
                : NodeType<double>(), id(OutputNode::nextId++)
        {}

        double activate () {
            return rawValue;
        }

        // Print to the screen for testing.
        void send () {
            std::cout << "Output Node ["
                      << id << "] "
                      << activate()
                      << std::endl;
        }

    };

}

int network::OutputNode::nextId = 0;


#endif //NNETWORK_OUTPUTNODE_HPP
