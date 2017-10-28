#include "Player.h"


void Player::findMoves(const vector<vector<Piece>> &gameBoard){
	for(int i = 0; i < pieces.size(); i++){
		vector < vector < point> > currentPieceMoves;
		currentPieceMoves = pieces.at(i).findMoves(gameBoard);
		possibleMoves.insert(possibleMoves.end(),currentPieceMoves.begin(),currentPieceMoves.end());
	}
}

void Player::movesToString(const vector<vector<Piece>> &gameBoard){
	
}

void Player::initializePlayer(){
	
}