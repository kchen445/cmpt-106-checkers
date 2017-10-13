//
// Created by Jeremy Schwartz, 10/13/17
//

#include "NodeType.hpp"

using namespace network;

NodeType::NodeType (char type)
        : type(type), rawValue(2), connections()
{}

NodeType::NodeType (char type, double const &rawValue)
        : type(type), rawValue(rawValue), connections()
{}

double NodeType::calculate () {
    if (rawValue >= -1 && rawValue <= 1) {
        return rawValue;
    } else {

        double calculatedValue = 0;
        for (auto conn : connections) {
            calculatedValue += conn.getValue();
        }

        rawValue = activationFunction(calculatedValue);
        return rawValue;
    }
}

void NodeType::addConnection (NodeType* node, double const &weight) {
    connections.push_back({node, weight});
}

char NodeType::getType () const {
    return type;
}
