//
// Created by kev on 11/10/17.
//

#include "HumanPlayer.h"

using namespace std;

HumanPlayer::HumanPlayer(int whichPlayer)
:player(whichPlayer),indexOfMove(0),canMove(true)
{

}

void HumanPlayer::getMove() {
	int flag = 0;

	while (flag != 1) {
		string input = "";
		cout << "Please enter your move";
		getline(cin, input);
		cout << "You entered: " << input << endl << endl;

		for (int i = 0; i < str_len; i++) {
			if (input.compare(moves[i])) {
				indexOfMove = i;
				flag = 1;
			}
		}
	}

	if (flag == 0) {
		cout << "invalid move, try again" << endl;
	}

}