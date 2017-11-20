//
// Created by kev on 11/10/17.
//
#include "../game/Player.h"
#include "NNetwork.hpp"
#include "../ml/sysml.hpp"

#ifndef CMPT_106_CHECKERS_GAME_AIPLAYER_H
#define CMPT_106_CHECKERS_GAME_AIPLAYER_H


struct AIPlayer: public Player, public ml::entity_t {
	std::shared_ptr<network::NetworkType<64, 32>> network;
	
	AIPlayer(int whichPlayer, std::shared_ptr<network::NetworkType<64, 32>> network);
	AIPlayer(int whichPlayer, const std::string &filename);	
	
	void getMove(const std::vector< std::vector<Piece> > &gameBoard);
    
    double get_value () const override;

};


#endif //CMPT_106_CHECKERS_GAME_AIPLAYER_H
