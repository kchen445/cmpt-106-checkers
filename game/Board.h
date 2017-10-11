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

    Board(): gameBoard(8, std::vector < Piece >(8,(point(0,0)))) {}

    bool positionIsEmpty(point point) const {
        return(gameBoard[point.row][point.col].isEmpty);
    }

    //function to create the starting conditions for checkers in case
    //player decides to have multiple games in one build
    void initializeBoard();

    //update the board after a player's move is taken
    void update(Player player);
};


#endif //CMPT_106_CHECKERS_GAME_BOARD_H
