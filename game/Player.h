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
	//variable to determine if a player object is player 1 or player 2
    int player;
	
	//rather than have a function that returns an int
	//this variable stores the index of whichever move 
	//the player chooses from the vector of possible moves
    int indexOfMove;
	
	//used to determine if a player can move on their turn
	//if not then set canMove to false and the game will end
    bool canMove;
	
	//a vector of all pieces that a player controls
	//call the findMoves function for each piece to find possible
	//moves
    std::vector< Piece > pieces;
	
	//a checker piece can have multiple jumps in different directions
	//the inner vector stores all the jumps a single piece makes
	//the outer vector stores the inner vectors as a move for a single turn
    std::vector< std::vector < line > > possibleMoves;
	
	//easiest way to get player input is to compare strings
	//convert the possibleMoves vector into strings that can
	//be easily outputted onto the terminal and easy to compare
    std::vector< std::string > movesAsString;

	//call the findMoves function for each piece in the vector of pieces
	//and push them to the possibleMoves vector
    virtual void findMoves(const Board &board) = 0;
	
	//convert the lines in possibleMoves to strings to put in
	//movesAsStrings
	virtual void movesToString() = 0;
	
	//set up the initial conditions for the player
	//by resetting variables like pieces, canMove, etc
    virtual void initializePlayer() = 0;
	
	//use cin to take in player input and check to see if the
	//input matches any of the strings in movesAsString
	//may be helpful to list out the elements of movesAsStrings
	//to show what choices are possible
    virtual void getMove() = 0;
	
    virtual~player() {}
};


#endif //CMPT_106_CHECKERS_GAME_PLAYER_H
