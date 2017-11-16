#include <iostream>
#include <string>
#include "Helpers.h"

enum class Tile: char {
    player1 = 'x', player2 = 'o', empty = ' ', player1King = 'X', player2King = 'O'
};

//function to test whether a given change will go out of bounds for the game board
bool withinBounds(int baseX, int baseY, int changeX, int changeY) {
    return ((baseY + changeY) < 8  && (baseY + changeY) >= 0 &&
            (baseX + changeX) < 8 && (baseX + changeX) >= 0);
}


Point::Point(int x, int y)
        :row(x),col(y)
{}
Point::Point(const Point &other)
        :row(other.row),col(other.col)
{}
Point::Point()
:row(0),col(0)
{}
bool isOdd(int x){
    return ((x%2) == 1);
}

bool operator== (Point a, Point b) {
    return (a.row == b.row && a.col == b.col);
}

//helper function to see where pieces should go
//pieces are positioned from top to bottom such that
//adding the row and column (0 indexed ) together of
//the Piece add up to an odd number

std::string pointToString(Point a) {
    std::string move = "";
    move.push_back(a.row + 1 +'0');
    char cCol = a.col + 1 + '0';
    move.push_back(',');
    move.push_back(cCol);
    return move;
}

//function that determines if the player wants to play another round
bool retry() {
    std::string playAgain;
    std::cout << "Game over! Play again? (Y/N)" << std::endl;
    while (true) {
        std::cin >> playAgain;
        if (playAgain.size() == 1) {
            if (playAgain.at(0) == 'Y' ||playAgain.at(0) == 'y') {
                return true;
            } else if(playAgain.at(0) == 'N' || playAgain.at(0) == 'n') {
                return false;
            } else {
                std::cout << "Invalid input" << std::endl;
            }
        } else {
            std::cout << "Invalid input" << std::endl;
        }
    }
}

//get input to see if the game will be human vs human or human vs ai
//return string to prevent errors/complications
std::string findGameMode(){
    std::string gameMode;
    while (true) {
        getline(std::cin,gameMode);
        if (gameMode.size() == 1) {
            if (gameMode.at(0) == '1') {
                return gameMode;
            } else if(gameMode.at(0) == '2') {
                return gameMode;
            } else {
                std::cout << "Invalid input" << std::endl;
            }
        } else {
            std::cout << "Invalid input" << std::endl;
        }
    }
}
