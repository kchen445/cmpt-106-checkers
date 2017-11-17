#include "AIPlayer.h"

#include "../ml/sysml.h"
#include <array>

AIPlayer::AIPlayer(ptr<ml::network_o> network) : network(network) {
	//no code
}

inline unsigned int index(int row, int col) {
	return row*4 + col/2;
}

void AIPlayer::getMove(const std::vector< std::vector<Piece> > &gameBoard){
	//format inputs for neural network
	std::array<double,64> inputs(0);
	for (int row=0; row<8; row++) {
		for (int col=1-row%2; col<8; col+=2) {
			Piece &piece = gameBoard[row][col];
		
			//if my piece on this tile, player = 1
			//if opponent piece on tile, player = -1
			double ismine = 2 * (player == piece.player) - 1;
			
			if (piece.isEmpty) {
				inputs[index(row, col)] = 0;
			} else {
				inputs[index(row, col)] = ismine;
				if (piece.isKing)
					inputs[32 + index(row, col)] = ismine;
			}
		}
	}
	
	//ask the AI
	std::array<double,32> outputs = network->solve(inputs);
	
	//pick the dest with the highest fitness value,
	//	and the orig with the lowest fitness value
	double dest_fitness = -1337;
	double orig_fitness = 1337;
	
	for (size_t i=0; i<possibleMoves.size(); i++) {
		//get fitness values of move start and move end
		Point &moveend = possibleMoves[i].back();
		Point &movestart = possibleMoves[i].front();
		double fitend = outputs[index(moveend.row, moveend.col)];
		double fitstart = outputs[index(movestart.row, movestart.col)];
		
		if (dest_fitness < fitend) {		//if current move end is more desirable
			dest_fitness = fitend;				//set current move
			orig_fitness = fitstart;
			indexOfMove = i;
		} else if (dest_fitness == fitend) {
			if (orig_fitness > fitstart) {	//if current move start is more desirable
				orig_fitness = fitstart;		//set current move
				indexOfMove = i;
			}
		}
	}
}