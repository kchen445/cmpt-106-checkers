//
// Created by Jeremy S on 2017-09-26.
//

#ifndef NNETWORK_THRESHOLDNODE_HPP
#define NNETWORK_THRESHOLDNODE_HPP

#include "NodeType.hpp"

namespace network {

    class ThresholdNode : public NodeType<double> {
    public:
        ThresholdNode ()
                : NodeType<double>('T')
        {}

        double activate () {
            return rawValue >= 0 ? 1 : -1;
        }

    };

}


#endif //NNETWORK_THRESHOLDNODE_HPP
