//
// Created by kev on 11/10/17.
//
#include <vector>
#include "Helpers.h"

#ifndef CMPT_106_CHECKERS_GAME_PIECE_H
#define CMPT_106_CHECKERS_GAME_PIECE_H

struct Piece {
    Point position;
    bool isKing;
    //determines which player the Piece belongs to
    int player;
    //board will be a 2d vector of checker objects
    //use this to determine whether a point on the board
    //has a proper Piece or not
    bool isEmpty;
	std::vector< std::vector<Point> > allJumps;
	
    Piece(Point initialPosition, int player1Or2);
    Piece(Point initialPosition);
	//helper functions to find piece jumps
	bool canJumpPiece(const std::vector<std::vector<Piece>> &gameBoard);
	bool canJumpKing (const std::vector<std::vector<Piece>> &gameBoard);
    void getJumpsPiece (std::vector < Point > jumps, const std::vector< std::vector < Piece > > &gameBoard);
	void getJumpsKing  (std::vector < Point > jumps, const std::vector<std::vector<Piece>> &gameBoard, Point direction);
	void getJumpsToEmptyPiece (const std::vector<std::vector<Piece>> &gameBoard);
	void getJumpsToEmptyKing  (const std::vector<std::vector<Piece>> &gameBoard);
    std::vector< std::vector<Point> > findMoves (const std::vector< std::vector < Piece > > &gameBoard);
};


#endif //CMPT_106_CHECKERS_GAME_PIECE_H
