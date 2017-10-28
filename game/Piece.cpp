//
// Created by kev on 11/10/17.
//

#include "Piece.h"
#include "Helpers.h"
 using namespace std;
	//list of piece variables
    //Point position;
    //bool isKing;
    //int player;
    //bool isEmpty;
    //vector< vector<Point> > allJumps;

Piece::Piece(Point initialPosition, int player1Or2){
	position = initialPosition;
	isKing = false;
	player = player1Or2;
	isEmpty = false;
}

Piece::Piece(Point initialPosition){
	position = initialPosition;
	isKing = false;
	player = 0;
	isEmpty = true;
}

bool Piece::canJumpPiece (const vector<vector<Piece>> &gameBoard){
	int row = position.row;
	int col = position.col;
	int rowAddition;
	
	this->player == 1 ? rowAddition = -1 : rowAddition = 1;
	bool canJump = false;
	
	for(int colAddition = -1; colAddition < 2; colAddition += 2){
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

bool Piece::canJumpKing (const vector<vector<Piece>> &gameBoard){
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

void Piece::getJumpsToEmptyPiece (){
	vector < Point > jumps;
	int row = position.row;
	int col = position.col;
	int rowAddition;
	this -> player == 1 ? rowAddition = -1 : rowAddition = 1;
	for(int colAddition = -1, colAddition < 2; colAddition += 2){
		if(withinBounds(row,col,rowAddition,colAddition){
			if(gameBoard.at(row + rowAddition).at(col+colAddition).isEmpty()){
				Point jumpDestination(row + rowAddition,col+colAddition);
				jumps.push_back(position);
				jumps.push_back(jumpDestination);
				allJumps.push_back(jumps);
			}
		}
	}
}
void Piece::getJumpsToEmptyKing  (const vector<vector<Piece>> &gameBoard){
	vector < Point > jumps;
	int row = position.row;
	int col = position.col;
	for(int rowAddition = -1, rowAddition < 2; rowAddition += 2){
		for(int colAddition = -1; colAddition < 2; colAddition +=2){	
			if(withinBounds(row,col,rowAddition,colAddition){
				if(gameBoard.at(row + rowAddition).at(col+colAddition).isEmpty()){
					Point jumpDestination(row+rowAddition,col+colAddition);
					jumps.push_back(position);
					jumps.push_back(jumpDestination);
					allJumps.push_back(jumps);
				}
			}
		}
	}
}

//helper functions to recursively find all possible moves for a particular piece
void Piece::getJumpsPiece (vector < Point > jumps, const vector< vector < Piece > > &gameBoard){
	bool hasJumped = false;
	int row = jumps.back().row;
	int col = jumps.back().col;
	int rowAddition;
	this -> player == 1 ? rowAddition = -1 : rowAddition = 1;
	for(int colAddition = -1; colAddition < 2; colAddition += 2){
		if(withinBounds(row,col,rowAddition,colAddition)){
			if(!gameBoard.at(row + rowAddition).at(col+colAddition).isEmpty()
				&&withinBounds(row,col,2*rowAddition,2*colAddition)){
				if(gameBoard.at(row + rowAddition).at(col + colAddition).player != this->player && 
				gameBoard.at(row + (2*rowAddition)).at(col+(2*colAddition)).isEmpty(){
					hasJumped = true;
					Point jumpDestination(row+(2*rowAddition),col+(2*colAddition));
					jumps.push_back(jumpDestination);
					getJumpsPiece(jumps,board);
				}
			}
		}
	}
	if(!hasJumped&&jumps.size()>1){
		allJumps.push_back(jumps);
	}
}
 
void Piece::getJumpsKing (vector < Point > jumps, const vector<vector<Piece>> &gameBoard, Point direction){
	bool hasJumped = false;
	int row = jumps.back().row;
	int col = jumps.back().col;
	for(int rowAddition = -1; rowAddition < 2; rowAddition += 2){
		for(int colAddition = -1; colAddition < 2; rowAddition += 2){
			if(withinBounds(row,col,rowAddition,colAddition)){
				if(!gameBoard.at(row + rowAddition).at(col+colAddition).isEmpty()
				&&withinBounds(row,col,2*rowAddition,2*colAddition)){
					if(gameBoard.at(row + rowAddition).at(col+colAddition).player != this->player && 
					gameBoard.at(row + (2*rowAddition)).at(col+(2*colAddition)).isEmpty(){
						hasJumped = true;
						Point direction(rowAddition,colAddition);
						Point jumpDestination(row+(2*rowAddition),newCol+colAddition);
						jumps.push_back(jumpDestination);
						getJumpsKing(jumps,gameBoard);
					}
				}
			}
		}
	}
	if(!hasJumped && jumps.size()>1){
		allJumps.push_back(jumps);
	}
}
 
vector< vector< Point > > Piece::findMoves (const vector<vector<Piece>> &gameBoard){
	allJumps.clear();
	if(isEmpty){ return allJumps; }
	if(!isKing){
		if(canJumpPiece){
			vector <Point> jumps;
			jumps.push_back(position);
			getJumpsPiece(jumps,gameBoard);
			return allJumps;
		}else{
			getJumpsToEmptyPiece(gameBoard);
			return allJumps;
		}
	//piece is a king
	}else{
		if(canJumpKing){
			vector <Point> jumps;
			jumps.push_back(position);
			getJumpsKing(jumps,gameBoard);
			return allJumps;
		}else{
			getJumpsToEmptyKing(gameBoard);
			return allJumps;
		}
	}
}