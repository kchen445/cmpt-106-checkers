#ifndef NNETWORK_NODETYPEEX_HPP
#define NNETWORK_NODETYPEEX_HPP

#include <vector>
#include "Connection.hpp"
#include "../core/Optional.hpp"

namespace network {

    template<typename T>
    class NodeTypeEx {
    public:

        double rawValue;

        std::vector<Connection<double, NodeTypeEx<double>>> connections;

        NodeTypeEx ()
                : rawValue(2), connections()
        {}

        NodeTypeEx (T const &rawValue)
                : rawValue(rawValue), connections()
        {}

        virtual T calculate () {
            if (rawValue >= -1 && rawValue <= 1) {
                return rawValue;
            } else {

                T calculatedValue = 0;
                for (auto conn : connections) {
                    calculatedValue += conn.getValue();
                }

                rawValue = activationFunction(calculatedValue);
                return rawValue;
            }
        }

        virtual T activationFunction (T const &calculatedValue) = 0;

        void addConnection (NodeTypeEx<T>* node, T const &weight) {
            connections.push_back({node, weight});
        }

    };

}

#endif
