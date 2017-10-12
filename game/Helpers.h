#include <string>
#include <iostream>

#ifndef CMPT_106_CHECKERS_GAME_HELPERS_H
#define CMPT_106_CHECKERS_GAME_HELPERS_H

enum class Tile: char;

//function to test whether a given change will go out of bounds for the game board
bool withinBounds(int baseY, int baseX, int changeY = 0, int changeX = 0);


struct point{
    int row;
    int col;
    point(int x, int y);
};

struct line{
    point start;
    point end;

    line(point start, point end);
};

//all pieces are on square where the row and column add up to 
//an odd number, this is a function to help make finding
//whether a number is odd or not easier and more readable
bool isOdd(int x);

bool operator== (point a, point b);



std::string pointToString(point a);

//function that determines if the player wants to play another round
bool retry();

//get input to see if the game will be human vs human or human vs ai
//return string to prevent errors/complications
std::string findGameMode();


#endif //CMPT_106_CHECKERS_GAME_HELPERS_H