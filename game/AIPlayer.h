//
// Created by kev on 11/10/17.
//

#ifndef CMPT_106_CHECKERS_GAME_AIPLAYER_H
#define CMPT_106_CHECKERS_GAME_AIPLAYER_H

#include "../ai/NNetwork.hpp"
#include "../ml/sysml.hpp"

#include "Player.h"

struct AIPlayer: public Player, public ml::entity_t {
	std::shared_ptr<ml::network_t<64, 32>> network;
	double rating;
	
	AIPlayer(std::shared_ptr<ml::network_t<64, 32>> network);
	
	AIPlayer(int whichPlayer, std::shared_ptr<ml::network_t<64, 32>> network, double rating = 100);
	AIPlayer(int whichPlayer, const std::string &filename);	
	
	void getMove(const std::vector< std::vector<Piece> > &gameBoard);
    
    double get_value () const override;

};


#endif //CMPT_106_CHECKERS_GAME_AIPLAYER_H
