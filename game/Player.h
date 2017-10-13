//
// Created by kev on 11/10/17.
//
#include "Helpers.h"
#include "Piece.h"
#include <vector>
#include <string>


#ifndef CMPT_106_CHECKERS_GAME_PLAYER_H
#define CMPT_106_CHECKERS_GAME_PLAYER_H


struct Player {
public:
    int player;
    int indexOfMove;
    bool canMove;
    std::vector< Piece > pieces;
    std::vector< std::vector < line > > possibleMoves;
    std::vector< std::string > movesToString;

    virtual void findMoves(std::vector < std::vector < Tile > > board) = 0;
    virtual void reinitializePlayer() = 0;
    virtual std::vector< Piece > getVectorOfPieces() = 0;
    virtual void findMoves() = 0;
    virtual void getMove() = 0;
    virtual~player() {}
};


#endif //CMPT_106_CHECKERS_GAME_PLAYER_H
