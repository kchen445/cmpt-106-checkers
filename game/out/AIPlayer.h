//
// Created by kev on 11/10/17.
//
#include "Player.h"
#ifndef CMPT_106_CHECKERS_GAME_AIPLAYER_H
#define CMPT_106_CHECKERS_GAME_AIPLAYER_H


struct AIPlayer: public Player{

	AIPlayer(int whichPlayer);
	
	void getMove(const std::vector< std::vector<Piece> > &gameBoard);

};


#endif //CMPT_106_CHECKERS_GAME_AIPLAYER_H
