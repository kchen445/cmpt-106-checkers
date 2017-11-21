//
// Created by kev on 11/10/17.
//

#include "AIPlayer.h"

#include <iostream>
#include <stdlib.h>
using namespace std;

AIPlayer::AIPlayer(int whichPlayer) : Player(whichPlayer)
{}

void AIPlayer::getMove(const std::vector< std::vector<Piece> > &gameBoard){
	/*std::cout << possibleMoves.size() << std::endl;
	
	if(possibleMoves.size() == 0){
		canMove = false;
		return;
	}

	indexOfMove = 0;*/
    if(possibleMoves.size()==0) {
        canMove = false;
        return;
    }
    movesToString();
	indexOfMove = rand() % possibleMoves.size();
	/*
    int flag = 0;

	while (flag != 1) {
		string input = "";
		cout << "Please enter your move: \n";
        printMoveStringVector();
		getline(cin, input);
		cout << "You entered: " << input << endl << endl;

		for (int i = 0; i < movesAsString.size(); i++) {
			if (input.compare(movesAsString[i]) == 0) {
				indexOfMove = i;
				cout << i << endl;
				flag = 1;
                break;
			}
		}
		if (flag == 0) {
			cout << "Invalid move, please try again." << endl;
		}
	}*/
}
