#ifndef NNETWORK_SIGMOIDNODE_HPP
#define NNETWORK_SIGMOIDNODE_HPP

#include "NodeType.hpp"

namespace network {

    class SigmoidNode : public NodeType {

        SigmoidNode ();

        double activationFunction (double in) override;

    };

}

#endif
