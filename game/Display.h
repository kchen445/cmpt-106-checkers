//
// Created by kev on 11/10/17.
//
#include "Helpers.h"
#include "Piece.h"
#include "Board.h"
#include <vector>
#ifndef CMPT_106_CHECKERS_GAME_DISPLAY_H
#define CMPT_106_CHECKERS_GAME_DISPLAY_H


struct Display{
public:

    std::vector < std::vector<std::string> > displayBoard;
	

    Display();
	
	//given the gameBoard, decide which of the tiles from the enumerated class
	//should go in a displayBoard spot
	void setUpBoard(const std::vector<std::vector <Piece> > &gameBoard);
	
	//actually display the board, along with the axis
    void displayGame(const std::vector<std::vector <Piece> > &gameBoard);

};

#endif //CMPT_106_CHECKERS_GAME_DISPLAY_H
