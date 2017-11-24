//
// Created by kev on 11/10/17.
//

#ifndef CMPT_106_CHECKERS_GAME_AIPLAYER_H
#define CMPT_106_CHECKERS_GAME_AIPLAYER_H

#include "../ai/NNetwork.hpp"
#include "../ml/sysml.hpp"

#include "Player.h"

struct AIPlayer: public Player, public ml::entity_t {
    double rating;
    double performance;
    
    explicit AIPlayer(std::shared_ptr<ml::network_o> const &network)
            : Player(1), ml::entity_t(network), rating(100)
    {}
    
    AIPlayer(int whichPlayer, std::shared_ptr<ml::network_o> network, double rating = 100);
    AIPlayer(int whichPlayer, const std::string &filename); 
    
    inline unsigned int index(int row, int col);
    
    void getMove(const std::vector< std::vector<Piece> > &gameBoard) override;
    
    double get_value () const override;

private:

    size_t num_turns;

};


#endif //CMPT_106_CHECKERS_GAME_AIPLAYER_H
