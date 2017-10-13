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

    HumanPlayer(int whichPlayer);
    void findMoves(const Board &board);
	void movesToString();
    void initializePlayer();
    void getMove();
};



#endif //CMPT_106_CHECKERS_GAME_HUMANPLAYER_H
