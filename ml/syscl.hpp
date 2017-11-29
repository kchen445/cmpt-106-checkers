//
// Created by Jeremy S on 2017-11-14.
//

#pragma once

#include <tuple>
#include <array>
#include "sysml.hpp"

namespace cl {
	
	struct game_data {
        int turns = -1;          			// The length of the game.
        int winner = -1;         			// who won?
        bool tie = false;          			// tie?
		std::array<char, 15> p1take{};	//pieces taken by p1 in 5-turn timeslices
		std::array<char, 15> p2take{};	//pieces taken by p2
		int p1taken = 0;
		int p2taken = 0;
    };

    // GameController should extend this class
    template<typename entity_t>
    class game_template {
    public:

        virtual game_data compete (entity_t &e1, entity_t &e2) = 0;

    };

}
