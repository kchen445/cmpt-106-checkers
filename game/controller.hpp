#pragma once

#include <iostream>
#include <vector>
#include <array>
#include "HumanPlayer.h"
#include "AIPlayer.h"
#include "Display.h"

#include "../ml/syscl.hpp" // for machine learning system
#include "../ml/ai_player.hpp"

// #define DISABLE_DISPLAY before including this file for training system

using namespace std;

//struct Data{
//    int turns;
//    bool win;
//    bool loss;
//    bool tie;
//    int piecesLost;
//    Data(int turns, bool win, bool loss, bool tie, int piecesLost)
//    :turns(turns),win(win),loss(loss),tie(tie),piecesLost(piecesLost)
//    {}
//};

using Data = cl::game_data;


struct CheckerController : public cl::game_template</*AIPlayer*/ rl::ai_player> {
	//order of player parameters matter
	//board will treat the first player passed in as player 1
	//and have their pieces at the bottom
    Data gameLoop(Player* player1, Player* player2);
    
    
    // Override from cl::game_template<AIPlayer>
    Data compete (rl::ai_player &e1, rl::ai_player &e2) override;
    
}; // struct CheckersController
