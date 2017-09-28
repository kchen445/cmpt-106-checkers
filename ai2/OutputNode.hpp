//
// Created by Jeremy S on 2017-09-26.
//

#ifndef NNETWORK_OUTPUTNODE_HPP
#define NNETWORK_OUTPUTNODE_HPP

#include "NodeType.hpp"
//#include <iostream>

namespace network {

    class OutputNode : public NodeType<double> {
    public:
        OutputNode()
                : NodeType<double>('O')
        {}

        double activate () {
            return rawValue;
        }

        void send () {
            /*std::cout << "Output Node ["
                      << id << "] "
                      << activate()
                      << std::endl;*/
        }

    };

}

#endif //NNETWORK_OUTPUTNODE_HPP
