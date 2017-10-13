//
// Created by kev on 11/10/17.
//
#include "Player.h"
#ifndef CMPT_106_CHECKERS_GAME_HUMANPLAYER_H
#define CMPT_106_CHECKERS_GAME_HUMANPLAYER_H


struct HumanPlayer: public Player {
public:
    //int to store the index of the players chosen move from a
    //vector storing all possible moves for a given turn

    std::vector< pieces > pieces;
    //inner vector refers to how a Piece can jump multiple times
    //by taking more than 1 Piece
    //outer vector is a list of all possible moves a player can make
    std::vector< std::vector < line > > possibleMoves;


    HumanPlayer(int whichPlayer);
    //set up a player's Piece vector
    void reinitializePlayer();
    std::vector< Piece > getVectorOfPieces();

    //find all possible moves by finding the possible moves
    //of each individual Piece and add all of it to a vector
    void findMoves();

    void getMove();
};



#endif //CMPT_106_CHECKERS_GAME_HUMANPLAYER_H
