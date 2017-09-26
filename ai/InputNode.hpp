//
// Created by Jeremy S on 2017-09-26.
//

#ifndef NNETWORK_INPUTNODE_HPP
#define NNETWORK_INPUTNODE_HPP

#include "NodeType.hpp"

namespace network {

    class InputNode : public NodeType<double> {
    public:

        double val;

        InputNode (double val)
                : NodeType<double>(0), val(val)
        {}

        double activate () {
            return val;
        }

        void send () {
            for (int i = 0; i < connections.size(); ++i) {
                connections[i].feedForward(val);
            }
        }

    };

}


#endif //NNETWORK_INPUTNODE_HPP
