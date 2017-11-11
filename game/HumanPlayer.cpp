//
// Created by kev on 11/10/17.
//

#include "HumanPlayer.h"

using namespace std;

HumanPlayer::HumanPlayer(int whichPlayer) : Player(whichPlayer){}

void HumanPlayer::getMove(const std::vector< std::vector<Piece> > &gameBoard) {
    movesToString();
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
				flag = 1;
                break;
			}
		}
		if (flag == 0) {
			cout << "Invalid move, please try again." << endl;
		}
	}

}