//
// Created by Jeremy S on 2017-09-26.
//

#ifndef NNETWORK_INPUTNODE_HPP
#define NNETWORK_INPUTNODE_HPP

#include "NodeType.hpp"

namespace network {

    class InputNode : public NodeType<double> {
    public:
        InputNode ()
                : NodeType<double>('I')
        {}

        InputNode (double val)
                : NodeType<double>('I', val)
        {}
		
		void setValue(double val) {
			rawValue = val;
		}
		
        double activate () {
            return rawValue;
        }

        void send () {
            for (int i = 0; i < connections.size(); ++i) {
                connections[i].feedForward(rawValue);
            }
        }

    };

}


#endif //NNETWORK_INPUTNODE_HPP
