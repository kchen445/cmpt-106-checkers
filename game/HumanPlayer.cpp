//
// Created by kev on 11/10/17.
//

#include "HumanPlayer.h"

using namespace std;

HumanPlayer::HumanPlayer(int whichPlayer)
:player(whichPlayer),indexOfMove(0),canMove(true)
{

}


void HumanPlayer::findMoves(const vector< vector<Piece> > &gameBoard) {
	possibleMoves.clear();
	for (int i = 0; i < pieces.size(); i++) {
		vector< vector< Point > > currPieceMoves;
		if (pieces.at(i).player == 0) { continue; }
		currPieceMoves = Piece::findMoves();
		possibleMoves.pushback(currPieceMoves);
	}
}

void HumanPlayer::movesToString() {
	movesAsString.clear();
	for (int i = 0; i < possibleMoves.size(); i++) {
		string point_str = "";
		for (int j = 0; j < possibleMoves[i].size(); j++) {
			
			if (point_str == "") {
				point_str += to_string(possibleMoves[i][j].row) + to_string(possibleMoves[i][j].col);
			}
			else {
				point_str += "-" + to_string(possibleMoves[i][j].row) + to_string(possibleMoves[i][j].col);
			}
		}
		movesAsString.push_back(point_str);
	}
}

void HumanPlayer::initializePlayer() {
	indexOfMove = -1;
	//canMove = ?;
	pieces.clear();
	possibleMoves.clear();
	movesAsString.clear();
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