//
// Created by kev on 11/10/17.
//

#include "Board.h"

void Board::initializeBoard (){
	for(int i = 0; i < 3; i++){
		for(int j = 0; j < 8; j++){
			point position(i,j);
			if(isOdd(i+j)){
				board.at(i).at(j) = Piece(position,2);
			}else{
				board.at(i).at(j) = Piece(position);
			}
		}
	}
	for(int i = 3; i < 5; i++){
		for(int j = 0; j < 8; j++){
			point position(i,j);
			board.at(i).at(j) = Piece(position);
		}
	}
	for(int i = 5; i < 8; i++){
		for(int j = 0; j < 8; j++){
			point position(i,j);
			if(isOdd(i+j)){
				board.at(i).at(j) = Piece(position,1);
			}else{
				board.at(i).at(j) = Piece(position);
			}
		}
	}
}