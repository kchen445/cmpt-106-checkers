//
// Created by kev on 11/10/17.
//

#include "AIPlayer.h"
AIPlayer::AIPlayer(int whichPlayer)
:player(whichPlayer),indexOfMove(0),canMove(true)
{}

void getMove(const std::vector< std::vector<Piece> > &gameBoard){
	if(possibleMoves.size() == 0){
		canMove = false;
		return;
	}
}