//
// Created by Jeremy S on 2017-09-26.
//

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

        OutputNode(size_t inputs)
                : NodeType<double>(inputs), id(OutputNode::nextId++)
        {}

        double activate () {
            return rawValue;
        }

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
