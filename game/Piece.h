//
// Created by kev on 11/10/17.
//
#include <vector>
#include "Helpers.h"

#ifndef CMPT_106_CHECKERS_GAME_PIECE_H
#define CMPT_106_CHECKERS_GAME_PIECE_H
//  x  x
//   []
//     x
//      o
//
//o is player one
//x is player two
//[] represents position after jump
//in scenario shown above, two possible moves can be made from
//the position after jump and therefore needs 2 vectors to represents
//branch struct represents directions in which a piece can jump
//given a direction
struct branch {
	point position;
	std::vector <point> listOfDirectionChanges;
}


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
	//helper functions to find piece jumps
	std::vector < line > getJumpsPiece (const Board &board, point position ,int rowDirection);
	std::vector < line > getJumpsKing  (const Board &board, point position ,int rowDirection, int colDirection);
    std::vector< std::vector<line> > findMoves (const Board &board);
};


#endif //CMPT_106_CHECKERS_GAME_PIECE_H
