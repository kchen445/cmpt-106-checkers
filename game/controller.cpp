#include <iostream>
#include <vector>
#include <array>
#include "HumanPlayer.h"
#include "AIPlayer.h"
#include "Display.h"

#include "../ml/syscl.hpp" // for machine learning system

// #define DISABLE_DISPLAY before including this file for training system

using namespace std;

//struct Data{
//    int turns;
//    bool win;
//    bool loss;
//    bool tie;
//    int piecesLost;
//    Data(int turns, bool win, bool loss, bool tie, int piecesLost)
//    :turns(turns),win(win),loss(loss),tie(tie),piecesLost(piecesLost)
//    {}
//};

using Data = cl::player_data;


struct CheckerController : public cl::game_template<AIPlayer> {
	//order of player parameters matter
	//board will treat the first player passed in as player 1
	//and have their pieces at the bottom
    array<Data,2> gameLoop(Player* player1, Player* player2){
#ifndef DISABLE_DISPLAY
        Display display;
#endif
        Board board;

        bool tie = false;
        int player1LostPieces = 0;
        int player2LostPieces = 0;
        int turns = 0;
        bool player1Loss = false;
        bool player2Loss = false;

        vector < Player*> listOfPlayers;
        listOfPlayers.push_back(player1);
        listOfPlayers.push_back(player2);

        listOfPlayers[0]->canMove = true;
        listOfPlayers[1]->canMove = true;
        listOfPlayers[0]->initializePlayer();
        listOfPlayers[1]->initializePlayer();
        board.initializeBoard();

        //actual game loop
        while (listOfPlayers[0]->canMove || listOfPlayers[1]->canMove) {

#ifndef DISABLE_DISPLAY
            display.displayGame(board.gameBoard);
#endif
            
#ifndef DISABLE_DISPLAY
            cout << "Player 1's move" << endl;
#endif
            listOfPlayers[0]->findMoves(board.gameBoard);

            //canMove is updated inside getMove
            listOfPlayers[0]->getMove(board.gameBoard);
            if(!listOfPlayers[0]->canMove){break;}
            board.update(listOfPlayers,0);
            
#ifndef DISABLE_DISPLAY
            cout << "Player 1 move: " << listOfPlayers[0]->movesAsString.at(listOfPlayers[0]->indexOfMove) << endl;
#endif

#ifndef DISABLE_DISPLAY
            display.displayGame(board.gameBoard);
#endif
            
#ifndef DISABLE_DISPLAY
            cout << "Player 2's move" << endl;
#endif
            listOfPlayers[1]->findMoves(board.gameBoard);
            listOfPlayers[1]->getMove(board.gameBoard);
            if(!listOfPlayers[1]->canMove){break;}
            board.update(listOfPlayers,1);
            
#ifndef DISABLE_DISPLAY
            cout << "Player 2 move: " << listOfPlayers[1]->movesAsString.at(listOfPlayers[1]->indexOfMove) << endl;
#endif

            turns++;
            //https://boardgames.stackexchange.com/questions/34659/how-many-turns-does-an-average-game-of-checkers-draughts-go-for
            //the average of over 9000 checker games last around 25 turns per color
            //if the more than 75 turns pass then I'll just consider that a stalemate
            if(turns == 75){
                break;
            }
        }

        player1LostPieces = listOfPlayers[0]->findLostPieces();
        player2LostPieces = listOfPlayers[1]->findLostPieces();
        
#ifndef DISABLE_DISPLAY
        if(!listOfPlayers[1]->canMove){
            cout << endl << "Player 1 wins!" << endl << endl;
        }else if (!listOfPlayers[0]->canMove){
            cout << endl << "Player 2 wins!" << endl << endl;
        }else{
            tie = true;
            if(player1LostPieces < player2LostPieces){
                listOfPlayers[1]->canMove = false;
                player2Loss = true;
                cout << "Stalemate with Player 1 taking more pieces!" << endl;
            }else if(player1LostPieces == player2LostPieces){
                listOfPlayers[0]->canMove = false;
                listOfPlayers[1]->canMove = false;
                cout << "Complete Tie" << endl;
            }else{
                listOfPlayers[0]->canMove = false;
                player1Loss = true;
                cout << "Stalemate with Player 2 taking more pieces!" << endl;
            }
        }
#endif

        Data dataPlayer1(turns,listOfPlayers[0]->canMove,player1Loss,tie,player1LostPieces);
        Data dataPlayer2(turns,listOfPlayers[1]->canMove,player2Loss,tie,player2LostPieces);
        array<Data,2> arr = {dataPlayer1,dataPlayer2};
        return arr;
    }
    
    
    // Override from cl::game_template<AIPlayer>c
    array<Data, 2> compete (AIPlayer const &e1, AIPlayer const &e2) override {
        return gameLoop(&e1, &e2);
    }
    
}; // struct CheckersController
