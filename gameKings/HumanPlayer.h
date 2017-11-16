//
// Created by kev on 11/10/17.
//
#include "Player.h"
#ifndef CMPT_106_CHECKERS_GAME_HUMANPLAYER_H
#define CMPT_106_CHECKERS_GAME_HUMANPLAYER_H


struct HumanPlayer: public Player {
public:
  	//variables inherited from player class
    //int player;
    //int indexOfMove;
    //bool canMove;
    //std::vector< Piece > pieces;
    //std::vector< std::vector < line > > possibleMoves;
    //std::vector< std::string > movesAsString;
	
	//commented functions should be implemented into player.h/cpp
	
	//have the constructor set canMove to true and player to the parameter
    HumanPlayer(int whichPlayer);
    //void findMoves(const Board &board);
	//void movesToString();
    //void initializePlayer();
    void getMove(const std::vector< std::vector<Piece> > &gameBoard);
};



#endif //CMPT_106_CHECKERS_GAME_HUMANPLAYER_H
