//
// Created by kev on 11/10/17.
//
#include "Player.h"
#include "../ai/NNetwork.hpp"

#ifndef CMPT_106_CHECKERS_GAME_AIPLAYER_H
#define CMPT_106_CHECKERS_GAME_AIPLAYER_H


struct AIPlayer: public Player{
	std::shared_ptr<network::NetworkType<64, 32>> network;
	
	AIPlayer(int whichPlayer, std::shared_ptr<network::NetworkType<64, 32>> network);
	AIPlayer(int whichPlayer, const std::string &filename);	
	
	void getMove(const std::vector< std::vector<Piece> > &gameBoard);

};


#endif //CMPT_106_CHECKERS_GAME_AIPLAYER_H
