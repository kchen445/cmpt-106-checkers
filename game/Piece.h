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
	std::vector< std::vector<point> > allJumps;
	
    Piece(point initialPosition, int player1Or2);
    Piece(point initialPosition);
	//helper functions to find piece jumps
	bool canJump(const Board &board);
	bool canJumpKing (const Board &board);
	void getJumpsPiece (const Board &board, int rowDirection);
	void getJumpsKing  (const Board &board, int rowDirection, int colDirection);
	void getJumpsToEmptyPiece (const Board &board);
	void getJumpsToEmptyKing  (const Board &board);
    std::vector< std::vector<point> > findMoves (const Board &board);
};


#endif //CMPT_106_CHECKERS_GAME_PIECE_H
