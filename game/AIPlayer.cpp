//
// Created by kev on 11/10/17.
//

#include "AIPlayer.h"
AIPlayer::AIPlayer(int whichPlayer) : Player(whichPlayer)
{}

void AIPlayer::getMove(const std::vector< std::vector<Piece> > &gameBoard){
	if(possibleMoves.size() == 0){
		canMove = false;
		return;
	}
}