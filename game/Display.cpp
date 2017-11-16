//
// Created by kev on 11/10/17.
//

#include "Display.h"

using namespace std;

Display::Display()
:board(8,vector<string>(8,"x"))
{}

void Display::setUpBoard(const std::vector<std::vector <Piece> > &gameBoard){
    string empty = " ";
    string player1 = "\033[31m\u25CF\033[0m";
    string player2 = "\033[34m\u25CF\033[0m";
    string player1King = "\033[31m\u272A\033[0m";
    string player2King = "\033[34m\u272A\033[0m";

    for(int row = 0; row < 8; row ++){
        for (int col = 0; col < 8; col++){
            if(gameBoard[row][col].player == 1){
                if(gameBoard[row][col].isKing){
                    board[row][col] = player1King;
                }else{
                    board[row][col] = player1;
                }
            }else{
                if(gameBoard[row][col].isKing){
                    board[row][col] = player2King;
                }else{
                    board[row][col] = player2;
                }
            }
            if(gameBoard[row][col].isEmpty) {board[row][col] = empty;}

        }
    }
}


void Display::displayGame(const std::vector<std::vector <Piece> > &gameBoard){
    setUpBoard(gameBoard);
    cout << "==================================================================================\n\n\n\n";
  	cout << "       0         1         2         3         4         5         6         7     \n";
  	cout << "  _________________________________________________________________________________\n";
  	cout << "  |         |         |         |         |         |         |         |         |\n";
  	cout << "  |         |         |         |         |         |         |         |         |\n";
  	cout << "0 |    " << board[0][0] << "    |    " << board[0][1] << "    |    " << board[0][2] << "    |    " << board[0][3] << "    |    " << board[0][4] << "    |    " << board[0][5] << "    |    " << board[0][6] << "    |    " << board[0][7] << "    |\n";
  	cout << "  |         |         |         |         |         |         |         |         |\n";
  	cout << "  |_________|_________|_________|_________|_________|_________|_________|_________|\n";
  	cout << "  |         |         |         |         |         |         |         |         |\n";
  	cout << "  |         |         |         |         |         |         |         |         |\n";
  	cout << "1 |    " << board[1][0] << "    |    " << board[1][1] << "    |    " << board[1][2] << "    |    " << board[1][3] << "    |    " << board[1][4] << "    |    " << board[1][5] << "    |    " << board[1][6] << "    |    " << board[1][7] << "    |\n";
  	cout << "  |         |         |         |         |         |         |         |         |\n";
  	cout << "  |_________|_________|_________|_________|_________|_________|_________|_________|\n";
  	cout << "  |         |         |         |         |         |         |         |         |\n";
  	cout << "  |         |         |         |         |         |         |         |         |\n";
  	cout << "2 |    " << board[2][0] << "    |    " << board[2][1] << "    |    " << board[2][2] << "    |    " << board[2][3] << "    |    " << board[2][4] << "    |    " << board[2][5] << "    |    " << board[2][6] << "    |    " << board[2][7] << "    |\n";
  	cout << "  |         |         |         |         |         |         |         |         |\n";
  	cout << "  |_________|_________|_________|_________|_________|_________|_________|_________|\n";
  	cout << "  |         |         |         |         |         |         |         |         |\n";
  	cout << "  |         |         |         |         |         |         |         |         |\n";
  	cout << "3 |    " << board[3][0] << "    |    " << board[3][1] << "    |    " << board[3][2] << "    |    " << board[3][3] << "    |    " << board[3][4] << "    |    " << board[3][5] << "    |    " << board[3][6] << "    |    " << board[3][7] << "    |\n";
  	cout << "  |         |         |         |         |         |         |         |         |\n";
  	cout << "  |_________|_________|_________|_________|_________|_________|_________|_________|\n";
  	cout << "  |         |         |         |         |         |         |         |         |\n";
  	cout << "  |         |         |         |         |         |         |         |         |\n";
  	cout << "4 |    " << board[4][0] << "    |    " << board[4][1] << "    |    " << board[4][2] << "    |    " << board[4][3] << "    |    " << board[4][4] << "    |    " << board[4][5] << "    |    " << board[4][6] << "    |    " << board[4][7] << "    |\n";
  	cout << "  |         |         |         |         |         |         |         |         |\n";
  	cout << "  |_________|_________|_________|_________|_________|_________|_________|_________|\n";
  	cout << "  |         |         |         |         |         |         |         |         |\n";
  	cout << "  |         |         |         |         |         |         |         |         |\n";
  	cout << "5 |    " << board[5][0] << "    |    " << board[5][1] << "    |    " << board[5][2] << "    |    " << board[5][3] << "    |    " << board[5][4] << "    |    " << board[5][5] << "    |    " << board[5][6] << "    |    " << board[5][7] << "    |\n";
  	cout << "  |         |         |         |         |         |         |         |         |\n";
  	cout << "  |_________|_________|_________|_________|_________|_________|_________|_________|\n";
  	cout << "  |         |         |         |         |         |         |         |         |\n";
  	cout << "  |         |         |         |         |         |         |         |         |\n";
  	cout << "6 |    " << board[6][0] << "    |    " << board[6][1] << "    |    " << board[6][2] << "    |    " << board[6][3] << "    |    " << board[6][4] << "    |    " << board[6][5] << "    |    " << board[6][6] << "    |    " << board[6][7] << "    |\n";
  	cout << "  |         |         |         |         |         |         |         |         |\n";
  	cout << "  |_________|_________|_________|_________|_________|_________|_________|_________|\n";
  	cout << "  |         |         |         |         |         |         |         |         |\n";
  	cout << "  |         |         |         |         |         |         |         |         |\n";
  	cout << "7 |    " << board[7][0] << "    |    " << board[7][1] << "    |    " << board[7][2] << "    |    " << board[7][3] << "    |    " << board[7][4] << "    |    " << board[7][5] << "    |    " << board[7][6] << "    |    " << board[7][7] << "    |\n";
  	cout << "  |         |         |         |         |         |         |         |         |\n";
  	cout << "  |_________|_________|_________|_________|_________|_________|_________|_________|\n";

}
