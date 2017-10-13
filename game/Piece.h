//
// Created by kev on 11/10/17.
//
#include <vector>
#include "Helpers.h"

#ifndef CMPT_106_CHECKERS_GAME_PIECE_H
#define CMPT_106_CHECKERS_GAME_PIECE_H


struct Piece {
    point position;
    bool isKing;
    //determines which player the Piece belongs to
    int player;
    //board will be a 2d vector of checker objects
    //use this to determine whether a point on the board
    //has a proper Piece or not
    bool isEmpty;

    Piece(point initialPosition, int player1Or2);
    Piece(point initialPosition);
    std::vector< std::vector<line> > findMoves (const Board &board);
};


#endif //CMPT_106_CHECKERS_GAME_PIECE_H
