//
// Created by kev on 11/10/17.
//

#include "Piece.h"
 using namespace std;
 
    //point position;
    //bool isKing;
    //int player;
    //bool isEmpty;
 
vector<  vector < line > > piece::getJumpsPiece (const Board &board, point position,int rowDirection){
	//for loop to run recursion, each recurse leads to a separate move
	int row = position.row;
	int col = position.col;
	
}
 
 vector < line > piece::getJumpsKing (const Board &board, point position, bool isKing){
	 
}
 
 vector< vector<line> > piece::findMoves (const Board &board){
	vector<line> currentJump;
	vector< vector<line> > allJumps;
	int row = position.row;
	int col = position.col;
	if(!isKing){
		while(canMove){
			//check for empty spots to a piece's upper diagonal right and left
			for(int rowAddition = -1; rowAddition < 2; rowAddition+=2){
				int newRow = row + rowAddition;
				int newCol = col + 1;
				if(board.gameBoard[newRow][newCol].isEmpty()){
					point end(newRow,newCol);
					point start(row,col);
					line move(start, end);
					currentJump.push_back(move);
					allJumps.push_back(currentJump);
				} else if (board.gameBoard[newRow][newCol] != this->player){
					vector< vector <lines> > allJumpsByPiece = getJumpsPiece(const &board, position, rowAddition);
					allJumps.insert(allJumps.end(), allJumpsByPiece.begin(), allJumpsByPiece.end());
				}
			}
		}
	//piece is a king
	}else{
		
	}
 }