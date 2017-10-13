#include <iostream>
#include <vector>
#include <string>
#include "Helpers.h"
#include "Piece.h"
#include "HumanPlayer.h"
#include "AIPlayer.h"
#include "Display.h"
#include "Board.h"

using namespace std;

const int PLAYER1 = 1;
const int PLAYER2 = 2;

int main() {
	bool stillPlay = true;
	Display display;
	Board gameBoard;
	vector< Player > listOfPlayers;
	cout << "1. Human Vs Human" << endl;
	cout << "2. Human Vs AI" << endl;
	cout << "Input your choice:" << endl;
	char gameMode = findGameMode().at(0);
	
	//add players to a vector of player objects
	//so it will be easier to change between a human vs human
	//and human vs ai mode
	if(gameMode == '1'){
		HumanPlayer player1(PLAYER1);
		HumanPlayer player2(PLAYER2);
		listOfPlayers.push_back(player1);
		listOfPlayers.push_back(player2);
	}else{
		HumanPlayer player1(PLAYER1);
		AIPlayer player2;
		listOfPlayers.push_back(player1);
		listOfPlayers.push_back(player2);
	}
	

  //this while loop creates the conditions necessary for a new game to be played
  while (stillPlay) {
    listOfPlayers[0].canMove = true;
    listOfPlayers[1].canMove = true;
	listOfPlayers[0].reinitializePlayer();
	listOfPlayers[1].reinitializePlayer();
    gameBoard.initializeBoard();
    cout << "X is player1" << endl;
    cout << "O is player2" << endl;
    cout << "Enter moves in the form of column,row i.e 6E" << endl;
    //actual game loop
    while (listOfPlayers[0].canMove || listOfPlayers[1].canMove) {
      cout << endl;
      if (listOfPlayers[1].possibleMoves.size()) {
        cout << "AI moved to " << listOfPlayers[1].movesToString.at(listOfPlayers[1].indexOfMove) << endl;
      }
      cout << "Player's move" << endl;
      display.displayGame(gameBoard.gameBoard);
      listOfPlayers[0].findMoves();
      listOfPlayers[0].getMove();
      gameBoard.update(listOfPlayers[0]);
      cout << endl;
      if (listOfPlayers[0].possibleMoves.size()) {
        cout << "You moved to " << listOfPlayers[0].movesToString.at(listOfPlayers[0].indexOfMove) << endl;
      }
      cout << "Computer's move" << endl;
      display.displayGame(gameBoard.gameBoard);
      listOfPlayers[1].findMoves();
      listOfPlayers[1].getMove();
      gameBoard.update(listOfPlayers[1]);
    }
    if(!listOfPlayers[1].canMove){
        cout << endl << "Player wins!" << endl << endl;
    }else if (!listOfPlayers[0].canMove){
        cout << endl << "Computer wins!" << endl << endl;
    }else{
        cout << endl << "Tie!" << endl;
    }
    stillPlay = retry();
  }
}
