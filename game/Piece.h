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
	std::vector< std::vector<Point> > allJumps;
	
    Piece(point initialPosition, int player1Or2);
    Piece(point initialPosition);
	//helper functions to find piece jumps
	bool canJump(const vector<vector<Piece>> &gameBoard);
	bool canJumpKing (const vector<vector<Piece>> &gameBoard);
	void getJumpsPiece (const vector<vector<Piece>> &gameBoard, int rowDirection);
	void getJumpsKing  (const vector<vector<Piece>> &gameBoard, int rowDirection, int colDirection);
	void getJumpsToEmptyPiece (const vector<vector<Piece>> &gameBoard);
	void getJumpsToEmptyKing  (const vector<vector<Piece>> &gameBoard);
    std::vector< std::vector<point> > findMoves (const vector< vector < Piece > > &gameBoard);
};


#endif //CMPT_106_CHECKERS_GAME_PIECE_H
