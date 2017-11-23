//
// Created by Jeremy S on 2017-11-14.
//

#pragma once

#include <tuple>
#include <array>
#include "sysml.hpp"

namespace cl {

    /*struct player_data {
        int turns;          // The length of the game.
        bool win;           // Did this player win?
        bool loss;          // lose?
        bool tie;           // tie?
        int piecesLost;     // The number of pieces this player lost (I think)
    };*/
	
	struct game_data {
        int turns = -1;          			// The length of the game.
        int winner = -1;         			// who won?
        bool tie = false;          			// tie?
		std::array<char, 15> p1take{};	//pieces taken by p1 in 5-turn timeslices
		std::array<char, 15> p2take{};	//pieces taken by p2
    };

    // GameController should extend this class
    template<typename entity_t>
    class game_template {
    public:

        //virtual std::array<player_data, 2> compete (entity_t &e1, entity_t &e2) = 0;
        virtual game_data compete (entity_t &e1, entity_t &e2) = 0;

    };

}
