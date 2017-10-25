//
// Created by kev on 11/10/17.
//

#include "Piece.h"
#include "Helpers.h"
 using namespace std;
	//list of piece variables
    //point position;
    //bool isKing;
    //int player;
    //bool isEmpty;
    //vector< vector<point> > allJumps;
// two functions to see if a particular piece can jump over another piece
// so we can ignore looking for empty spaces ahead if it can

Piece::Piece(point initialPosition, int player1Or2){
	position = initialPosition;
	isKing = false;
	player = player1Or2;
	isEmpty = false;
}

Piece::Piece(point initialPosition){
	position = initialPosition;
	isKing = false;
	player = 0;
	isEmpty = true;
}

bool Piece::canJumpPiece (const Board &board){
	int row = position.row;
	int col = position.col;
	int colAddition;
	
	this->player == 1 ? colAddition = -1 : colAddition = 1;
	bool canJump = false;
	
	for(int rowAddition = -1; rowAddition < 2; rowAddition += 2){
		if(withinBounds(row,col,rowAddition,colAddition)){
			if(withinBounds(row,col,2*rowAddition,2*colAddition)&&
			!board.gameBoard.at(row+rowAddition).at(col+colAddition).isEmpty()){
				if(board.gameBoard.at(row+rowAddition).at(col+colAddition).player != this->player&&
				board.gameBoard.at(row+(2*rowAddition)).at(col+(2*colAddition)).isEmpty()){
					return true;
				}
			}
		}
	}
	return canJump;
}

bool Piece::canJumpKing (const Board &board){
	int row = position.row;
	int col = position.col
	bool canJump = false;
	for(int rowAddition = -1; rowAddition < 2; rowAddition += 2){
		for(int colAddition = -1; colAddition < 2; rowAddition += 2){
			if(withinBounds(row,col,rowAddition,colAddition)){
				if(withinBounds(row,col,2*rowAddition,2*colAddition)&&
				!board.gameBoard.at(row+rowAddition).at(col+colAddition).isEmpty()){
					if(board.gameBoard.at(row+rowAddition).at(col+colAddition).player != this->player&&
					board.gameBoard.at(row+(2*rowAddition)).at(col+(2*colAddition)).isEmpty()){
							return canJump = true;
					}
				}
			}
		}
	}
	
	return canJump;
}

void Piece::getJumpsToEmptyPiece (const Board &board){
	
}
void Piece::getJumpsToEmptyKing  (const Board &board){
	
}

//helper functions to recursively find all possible moves for a particular piece
void Piece::getJumpsPiece (vector < point > jumps, const Board &board){
	bool hasJumped = false;
	int row = jumps.back().row;
	int col = jumps.back().col;
	int colAddition;
	this->player == 1 ? colAddition = -1 : colAddition = 1;
	int rowLeft = -1;
	int rowRight = 1;
	int newCol = col + colAddition;
	if(withinBounds(row,col,rowLeft,colAddition)){
		if(!board.gameBoard.at(row + rowLeft).at(newCol).isEmpty()
			&&withinBounds(row,col,2*rowLeft,2*colAddition)){
			if(board.gameBoard.at(row + rowLeft).at(newCol).player != this->player && 
			board.gameBoard.at(row + (2*rowLeft)).at(newCol+colAddition).isEmpty(){
				hasJumped = true;
				point jumpDestination(rowLeft-1,newCol+colAddition);
				jumps.push_back(jumpDestination);
				getJumpsPiece(jumps,board);
			}
		}
	}
	if(withinBounds(row,col,rowRight,colAddition)){
		if(!board.gameBoard.at(row + rowRight).at(newCol).isEmpty()
			&&withinBounds(row,col,2*rowLeft,2*colAddition)){
			if(board.gameBoard.at(row + rowRight).at(newCol).player != this->player && 
			board.gameBoard.at(row + (2*rowRight)).at(newCol+colAddition).isEmpty(){
				hasJumped = true;
				point jumpDestination(rowRight+1,newCol+colAddition);
				jumps.push_back(jumpDestination);
				getJumpsPiece(jumps,board);
			}
		}
	}
	if(!hasJumped){
		allJumps.push_back(jumps);
	}
}
 
void Piece::getJumpsKing (vector < point > jumps, const Board &board){
	bool hasJumped = false;
	int row = jumps.back().row;
	int col = jumps.back().col;
	for(int rowAddition = -1; rowAddition < 2; rowAddition += 2){
		for(int colAddition = -1; colAddition < 2; rowAddition += 2){
			if(withinBounds(row,col,rowRight,colAddition)){
				if(!board.gameBoard.at(row + rowRight).at(newCol).isEmpty()
				&&withinBounds(row,col,2*rowLeft,2*colAddition)){
					if(board.gameBoard.at(row + rowRight).at(newCol).player != this->player && 
					board.gameBoard.at(row + (2*rowRight)).at(newCol+colAddition).isEmpty(){
						hasJumped = true;
						point jumpDestination(rowRight+1,newCol+colAddition);
						jumps.push_back(jumpDestination);
						getJumpsPiece(jumps,board);
					}
				}
			}
		}
	}
}
 
 vector< vector< point > > Piece::findMoves (const Board &board){
	allJumps.clear();
	if(!isKing){
		if(canJumpPiece){
			vector <point> jumps;
			jumps.push_back(position);
			getJumpsPiece(jumps,board);
			return allJumps;
		}else{
			getJumpsToEmptyPiece(board);
			return allJumps;
		}
	//piece is a king
	}else{
		if(canJumpPiece){
			vector <point> jumps;
			jumps.push_back(position);
			getJumpsPiece(jumps,board);
			return allJumps;
		}else{
			getJumpsToEmptyPiece(board);
			return allJumps;
		}
	}
 }