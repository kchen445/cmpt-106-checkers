//
// Created by kev on 11/10/17.
//
#include <vector>
#include "Helpers.h"
#include "Piece.h"
#include "Player.h"

#ifndef CMPT_106_CHECKERS_GAME_BOARD_H
#define CMPT_106_CHECKERS_GAME_BOARD_H


struct Board {
public:
    std::vector < std::vector < Piece > > gameBoard;

    Board();
	
	//test whether a piece has its 'isEmpty' variable as true
    bool positionIsEmpty(const Point &point);

    //function to create the starting conditions for checkers in case
    //player decides to have multiple games in one build
    void initializeBoard();

    //after a player's move is taken in, change the board to reflect that
    void update(std::vector<Player*> listOfPlayers, int whichPlayer);

};


#endif //CMPT_106_CHECKERS_GAME_BOARD_H
