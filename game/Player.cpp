#include "Player.h"
#include "Helpers.h"

using namespace std;

Player::Player(int whichPlayer)
:player(whichPlayer),indexOfMove(0),canMove(true)
{};

void Player::findMoves(const std::vector<std::vector<Piece>> &gameBoard){
    possibleMoves.clear();
    for(int i = 0; i < pieces.size(); i++){

        std::vector < std::vector < Point> > currentPieceMoves;

		if(pieces.at(i).player == 0){continue;}
		currentPieceMoves = pieces.at(i).findMoves(gameBoard);
		possibleMoves.insert(possibleMoves.end(),currentPieceMoves.begin(),currentPieceMoves.end());
	}
}

int Player::findLostPieces(){
    int count=0;
    for(int i = 0; i < pieces.size();i++){
        if(pieces.at(i).isEmpty){count++;}
    }
    return count;
}

void Player::movesToString(){
	movesAsString.clear();
	for (int i = 0; i < possibleMoves.size(); i++) {
		std::string point_str = "";
		for (int j = 0; j < possibleMoves[i].size(); j++) {
			
			if (point_str == "") {
				point_str += std::to_string(possibleMoves[i][j].row) + std::to_string(possibleMoves[i][j].col);
            }
			else {
				point_str += "-" + std::to_string(possibleMoves[i][j].row) + std::to_string(possibleMoves[i][j].col);

			}
		}

		movesAsString.push_back(point_str);
	}
}

void Player::initializePlayer(){
	indexOfMove = -1;
	pieces.clear();
	possibleMoves.clear();
	movesAsString.clear();
	int startingRow;
	player == 2 ? startingRow = 0: startingRow = 5;

	for (int row = startingRow; row < startingRow + 3; row++){
		for (int col = 0; col < 8; col++){
			if(isOdd(row+col)){
				Piece playerPiece(Point(row,col),player);
				pieces.push_back(playerPiece);
			}
		}
	}
}


void Player::printMoveStringVector(){
    for(int i = 0; i < movesAsString.size(); i++){
        std::cout << movesAsString[i] <<", ";
    }
    std::cout << std::endl;
}
