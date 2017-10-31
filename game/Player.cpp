#include "Player.h"


void Player::findMoves(const std::vector<std::vector<Piece>> &gameBoard){
    possibleMoves.clear();
	for(int i = 0; i < pieces.size(); i++){
		std::vector < std::vector < Point> > currentPieceMoves;
		if(pieces.at(i).player == 0){continue;}
		currentPieceMoves = pieces.at(i).findMoves(gameBoard);
		possibleMoves.insert(possibleMoves.end(),currentPieceMoves.begin(),currentPieceMoves.end());
	}
}

void Player::movesToString(const std::vector<std::vector<Piece>> &gameBoard){
	
}

void Player::initializePlayer(){
	
}