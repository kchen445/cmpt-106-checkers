//
// Created by kev on 11/10/17.
//

#include "Display.h"

using namespace std;

Display::Display()
:displayBoard(8,vector<string>(8,"x"))
{}

void Display::setUpBoard(const std::vector<std::vector <Piece> > &gameBoard){
    string empty = "*";
    string player1 = "\033[31m1\033[0m";
    string player2 = "\033[34m2\033[0m";
    string player1King = "\033[1;4;31m1\033[0m";
    string player2King = "\033[1;4;34m2\033[0m";

    for(int row = 0; row < 8; row ++){
        for (int col = 0; col < 8; col++){
            if(gameBoard[row][col].player == 1){
                if(gameBoard[row][col].isKing){
                    displayBoard[row][col] = player1King;
                }else{
                    displayBoard[row][col] = player1;
                }
            }else{
                if(gameBoard[row][col].isKing){
                    displayBoard[row][col] = player2King;
                }else{
                    displayBoard[row][col] = player2;
                }
            }
            if(gameBoard[row][col].isEmpty) {displayBoard[row][col] = empty;}

        }
    }
}


void Display::displayGame(const std::vector<std::vector <Piece> > &gameBoard){
    setUpBoard(gameBoard);
    cout << "  ";
    for(int i = 0;i < 8; i ++){
        cout <<" "<< i<<" ";
    }
    cout << endl;
    for (int row = 0; row < 8; row++){
        cout << row <<":";
        for(int col = 0; col < 8; col++){
            cout << " " << displayBoard[row][col] << " ";
        }
        cout << endl;
    }
}
