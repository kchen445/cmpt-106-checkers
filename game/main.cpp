// This is the main source file for running the game.

#include <iostream>
#include <vector>
#include <string>
#include "Helpers.h"
#include "Piece.h"
#include "HumanPlayer.h"
#include "controller.hpp"
#include "AIPlayer.h"
#include "Display.h"
#include "Board.h"

using namespace std;

const int PLAYER1 = 1;
const int PLAYER2 = 2;

int main() {

    CheckerController controller;
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
		player2 = new AIPlayer(PLAYER2, "./bin/net.txt");
		//listOfPlayers.push_back(player1);
		//listOfPlayers.push_back(player2);
	}

  //this while loop creates the conditions necessary for a new game to be played
  while (stillPlay) {
	  controller.gameLoop(player1,player2);
	  stillPlay = retry();
  }
}
