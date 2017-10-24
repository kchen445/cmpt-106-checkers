#include "SigmoidNode.hpp"
#include <cmath>

using namespace network;

SigmoidNode::SigmoidNode()
        : NodeType('S')
          threshold(0)
{}

double SigmoidNode::activationFunction(double in) {
    return tanh(in);
}
