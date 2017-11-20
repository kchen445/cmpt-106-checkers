//
// Created by Jeremy S on 2017-11-14.
//

#pragma once

#include <tuple>
#include <array>
#include "sysml.hpp"

namespace cl {

    struct player_data {
        int turns;          // The length of the game.
        bool win;           // Did this player win?
        bool loss;          // lose?
        bool tie;           // tie?
        int piecesLost;     // The number of pieces this player lost (I think)
    };

    // GameController should extend this class
    template<typename entity_t>
    class game_template {
    public:

        virtual std::array<player_data, 2> compete (entity_t const &e1, entity_t const &e2) = 0;

    };

}
