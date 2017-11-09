#pragma once

#include <memory>
#include "CLOutcome.hpp"

namespace ml {

    // Super class for any game controller class that will be used for training.
    template<class Player>
    class CLGameTemplate {
    public:

        virtual CLDualOutcome compete (std::shared_ptr<Player> p1, std::shared_ptr<Player> p2) = 0;

    };

}
