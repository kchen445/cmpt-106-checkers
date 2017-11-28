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
	//Display display;
	//Board board;
	//vector< Player* > listOfPlayers;
	cout << "1. Human Vs Human" << endl;
	cout << "2. Human Vs AI" << endl;
	cout << "Input your choice:" << endl;
	char gameMode = findGameMode().at(0);
	
	//add players to a vector of player objects
	//so it will be easier to change between a human vs human
	//and human vs ai mode
	Player *player1, *player2;
	if(gameMode == '1'){
		player1 = new HumanPlayer(PLAYER1);
		player2 = new HumanPlayer(PLAYER2);
		//listOfPlayers.push_back(player1);
		//listOfPlayers.push_back(player2);
	}else{
		player1 = new HumanPlayer(PLAYER1);
		player2 = new AIPlayer(PLAYER2, "../bin/net.txt");
		//listOfPlayers.push_back(player1);
		//listOfPlayers.push_back(player2);
	}

  //this while loop creates the conditions necessary for a new game to be played
  while (stillPlay) {
	  controller.gameLoop(player1,player2);
	  stillPlay = retry();
  }
  /*
    listOfPlayers[0]->canMove = true;
    listOfPlayers[1]->canMove = true;
	listOfPlayers[0]->initializePlayer();
	listOfPlayers[1]->initializePlayer();
    board.initializeBoard();
	int turns = 0;
    cout << "Enter moves in the form of row,column-row,column i.e 64-53" << endl;
    //actual game loop
    while (listOfPlayers[0]->canMove || listOfPlayers[1]->canMove) {
      
      display.displayGame(board.gameBoard);
      cout << "Player 1's move" << endl;
      listOfPlayers[0]->findMoves(board.gameBoard);

	  //canMove is updated inside getMove
	  listOfPlayers[0]->getMove(board.gameBoard);
      if(!listOfPlayers[0]->canMove){break;}
	  board.update(listOfPlayers,0);
      cout << endl;

	  cout << "Player 1 move: " << listOfPlayers[0]->movesAsString.at(listOfPlayers[0]->indexOfMove) << endl;

      display.displayGame(board.gameBoard);
	  cout << "Player 2's move" << endl;
      listOfPlayers[1]->findMoves(board.gameBoard);
      listOfPlayers[1]->getMove(board.gameBoard);
      if(!listOfPlayers[1]->canMove){break;}
	  board.update(listOfPlayers,1);

	  cout << "Player 2 move: " << listOfPlayers[1]->movesAsString.at(listOfPlayers[1]->indexOfMove) << endl;
	
	
	  turns++;
	  //https://boardgames.stackexchange.com/questions/34659/how-many-turns-does-an-average-game-of-checkers-draughts-go-for
	  //the average of over 9000 checker games last around 25 turns per player 
	  //if the more than 75 turns pass then I'll just consider that a stalemate
	  if(turns == 75){
		  break;
	  }
	}
      int player1LostPieces = listOfPlayers[0]->findLostPieces();
      int player2LostPieces = listOfPlayers[1]->findLostPieces();
      if(!listOfPlayers[1]->canMove){
          cout << endl << "Player 1 wins!" << endl << endl;
      }else if (!listOfPlayers[0]->canMove){
          cout << endl << "Player 2 wins!" << endl << endl;
      }else{
          if(player1LostPieces < player2LostPieces){
              listOfPlayers[1]->canMove = false;
              cout << "Stalemate with Player 1 taking more pieces!" << endl;
          }else if(player1LostPieces == player2LostPieces){
              listOfPlayers[0]->canMove = false;
              listOfPlayers[1]->canMove = false;
              cout << "Complete Tie" << endl;
          }else{
              listOfPlayers[0]->canMove = false;
              cout << "Stalemate with Player 2 taking more pieces!" << endl;
          }
      }
    stillPlay = retry();
  }*/
}
