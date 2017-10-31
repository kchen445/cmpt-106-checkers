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
	Board board;
	vector< *Player > listOfPlayers;
	cout << "1. Human Vs Human" << endl;
	cout << "2. Human Vs AI" << endl;
	cout << "Input your choice:" << endl;
	char gameMode = findGameMode().at(0);
	
	//add players to a vector of player objects
	//so it will be easier to change between a human vs human
	//and human vs ai mode
	if(gameMode == '1'){
		Player* player1 = new HumanPlayer(PLAYER1);
		Player* player2 = new HumanPlayer(PLAYER2);
		listOfPlayers.push_back(&player1);
		listOfPlayers.push_back(&player2);
	}else{
		Player* player1 = new HumanPlayer(PLAYER1);
		Player* player2 = new AIPlayer(PLAYER2);
		listOfPlayers.push_back(&player1);
		listOfPlayers.push_back(&player2);
	}
	

  //this while loop creates the conditions necessary for a new game to be played
  while (stillPlay) {
    listOfPlayers[0]->canMove = true;
    listOfPlayers[1]->canMove = true;
	listOfPlayers[0]->initializePlayer();
	listOfPlayers[1]->initializePlayer();
    board.initializeBoard();
    cout << "X is player1" << endl;
    cout << "O is player2" << endl;
    cout << "Enter moves in the form of row,column-row,column i.e 6E-5F" << endl;
    //actual game loop
    while (listOfPlayers[0]->canMove || listOfPlayers[1]->canMove) {
      cout << endl;
      if (listOfPlayers[1]->possibleMoves.size()) {
        cout << "AI moved to " << listOfPlayers[1]->movesToString.at(listOfPlayers[1]->indexOfMove) << endl;
      }
      cout << "Player's move" << endl;
      display.displayGame(board.gameBoard);
      listOfPlayers[0]->findMoves(board.gameBoard);
      listOfPlayers[0]->getMove(board);
      board.update(listOfPlayers,0);
      cout << endl;
      if (listOfPlayers[0]->possibleMoves.size()) {
        cout << "You moved to " << listOfPlayers[0]->movesToString.at(listOfPlayers[0]->indexOfMove) << endl;
      }
      cout << "Computer's move" << endl;
      display.displayGame(board.gameBoard);
      listOfPlayers[1]->findMoves(board.gameBoard);
      listOfPlayers[1]->getMove(board);
      board.update(listOfPlayers,1);
    }
    if(!listOfPlayers[1]->canMove){
        cout << endl << "Player wins!" << endl << endl;
    }else if (!listOfPlayers[0]->canMove){
        cout << endl << "Computer wins!" << endl << endl;
    }else{
        cout << endl << "Tie!" << endl;
    }
    stillPlay = retry();
  }
}
