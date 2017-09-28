//
// Created by Jeremy S on 2017-09-26.
//

#ifndef NNETWORK_NODE_HPP
#define NNETWORK_NODE_HPP

#include <vector>
#include "ConnectionType.hpp"

namespace network {

    template<typename T>
    class NodeType {
    public:
		char type;
        T rawValue;
        std::vector<ConnectionType<T, NodeType<T>>> connections;

        NodeType (char type)
                : type(type), rawValue(0), connections()
        {}

        NodeType (char type, T val)
                : type(type), rawValue(val), connections()
        {}
		
        NodeType (NodeType<T> const &other)
                : type(other.type),
				  rawValue(other.rawValue),
                  connections(other.connections)
        {}

        void addConnection (NodeType<T>* node, double weight) {
            connections.push_back({node, weight});
        }

        virtual T activate () = 0;

        virtual void send () {
            T value = activate();
            for (auto connection : connections) {
                connection.feedForward(value);
            }
        }

    };

}


#endif //NNETWORK_NODE_HPP
