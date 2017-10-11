//
// Created by kev on 11/10/17.
//
#include "Helpers.h"
#include "Piece.h"
#include <vector>
#ifndef CMPT_106_CHECKERS_GAME_DISPLAY_H
#define CMPT_106_CHECKERS_GAME_DISPLAY_H


struct Display{
public:
    std::vector < std::vector<Tile> > displayBoard;

    Display();
    void displayGame(const std::vector <std::vector <Piece>> &gameBoard);
};

#endif //CMPT_106_CHECKERS_GAME_DISPLAY_H
