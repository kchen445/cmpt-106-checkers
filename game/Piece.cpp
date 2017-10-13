//
// Created by kev on 11/10/17.
//

#include "Piece.h"
 using namespace std;
 
    //point position;
    //bool isKing;
    //int player;
    //bool isEmpty;
 
 vector< vector<line> > piece::findMoves (const Board &board){
	vector<line> currentJump;
	vector< vector<line> > allJumps;
	point start(position.row, position.col);
	point end;
	line move;
	int numberOfJumps = 0;
	bool canMove = true;
	int row = position.row;
	int col = position.col;
	while(canMove){
		numberOfJumps++;
		int colAddition = numberOfJumps;
		for(int rowAddition = -1; rowAddition < 2; rowAddition+=2){
			if(board.gameBoard[row+(rowAddition*numberOfJumps)][col+colAddition].isEmpty()){
				
			}
		}
	}
 }