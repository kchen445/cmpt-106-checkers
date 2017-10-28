//
// Created by kev on 11/10/17.
//

#include "Board.h"

Board::Board()
:gameBoard(8,vector<Piece>(8,Piece(Point(0,0)))

//set up the initial positions of checker pieces
void Board::initializeBoard (){
	for(int i = 0; i < 3; i++){
		for(int j = 0; j < 8; j++){
			Point position(i,j);
			if(isOdd(i+j)){
				board.at(i).at(j) = Piece(position,2);
			}else{
				board.at(i).at(j) = Piece(position);
			}
		}
	}
	for(int i = 3; i < 5; i++){
		for(int j = 0; j < 8; j++){
			Point position(i,j);
			board.at(i).at(j) = Piece(position);
		}
	}
	for(int i = 5; i < 8; i++){
		for(int j = 0; j < 8; j++){
			Point position(i,j);
			if(isOdd(i+j)){
				board.at(i).at(j) = Piece(position,1);
			}else{
				board.at(i).at(j) = Piece(position);
			}
		}
	}
}

bool Board::positionIsEmpty(const Point &point){
	if(withinBounds(point.row,point.col)){
		return gameBoard.at(row).at(col).isEmpty;
	}
}

//function to take a player's move and apply it to the game board
void update(vector<*Player> listOfPlayers, int whichPlayer){
	int otherPlayer;
	whichPlayer == 0 ? otherPlayer = 1 : otherPlayer = 0;
	
	vector < Point > chosenMove(listOfPlayers[whichPlayer]->possibleMoves.at(listOfPlayers[whichPlayer]->indexOfMove));
	
	int indexOfPieceToBeMoved;
	for(int i = 0;i < listOfPlayers[whichPlayer]->pieces.size(); i++){
		if(chosenMove.at(0) == listOfPlayers[whichPlayer]->pieces.at(i).position){
			indexOfPieceToBeMoved = i;
			break;
		}
	}
	//when updating the board, it is also necessary to update the piece stored inside
	//a player struct
	*Piece pieceToBeMoved = &(listOfPlayers[whichPlayer]->pieces.at(indexOfPieceToBeMoved));
	
	//a jump that takes a piece will result in a position difference of +-2 for row and column
	//of initial Point and end Point
	bool moveTakesPiece = (chosenMove.at(1).row - chosenMove.at(0).row)%2 == 0);
	
	for(int i = 0, i < chosenMove.size()-1; i++){
		
		Point initialPosition = chosenMove.at(i);
		Point endPosition = chosenMove.at(i+1);
		
		//this block handles when a piece takes another piece
		if(moveTakesPiece){
			//find the relevant information of the other player's piece
			Point positionDifference(endPosition.row-initialPosition.row,endPosition.col-initialPosition.col);
			int otherPlayerPieceRow = initialPosition.row+(positionDifference.row/2);
			int otherPlayerPieceCol = initialPosition.col+(positionDifference.col/2);
			Point otherPlayerPiecePosition(otherPlayerPieceRow,otherPlayerPieceCol);
			int indexOfOtherPlayerPiece;
			for(int j = 0; j < listOfPlayers[whichPlayer]->pieces.size(); j++){
				if(otherPlayerPiecePosition == listOfPlayers[otherPlayer]->pieces.at(i).position){
					indexOfOtherPlayerPiece = i;
					break;
				}
			}
			
			//change the board by removing the other player's piece from the board
			//and remove this element from the vector of pieces for the other player
			board.at(otherPlayerPieceRow).at(otherPlayerPieceCol).isEmpty = true;
			board.at(otherPlayerPieceRow).at(otherPlayerPieceCol).player = 0;
			*Vector< Piece > otherPlayerPieces = &(listOfPlayers[otherPlayer]->pieces);
			OtherPlayerPieces->erase(*(otherPlayerPieces.begin())+i-1);
			
			//change the board according to a player's chosen move
			board.at(endPosition.row).at(endPosition.col) = board.at(initialPosition.row).at(initialPosition.col);
			board.at(endPosition.row).at(endPosition.col).position = endPosition;
			pieceToBeMoved->position = endPosition;
			board.at(initialPosition.row).at(initialPosition.col).isEmpty = true;
			board.at(initialPosition.row).at(initialPosition.col).player = 0;
			
			
		}
		//this block handles when a piece moves to an empty space
		else{
			board.at(endPosition.row).at(endPosition.col) = board.at(initialPosition.row).at(initialPosition.col);
			board.at(endPosition.row).at(endPosition.col).position = endPosition;
			pieceToBeMoved->position = endPosition;
			board.at(initialPosition.row).at(initialPosition.col).isEmpty = true;
			board.at(initialPosition.row).at(initialPosition.col).player = 0;
		}
		int rowToBeKinged;
		whichPlayer == 0 ? rowToBeKinged = 0 : rowToBeKinged = 7;
		if(endPosition.row == rowToBeKinged){
			pieceToBeMoved->isKing = true;
		}
	}
}