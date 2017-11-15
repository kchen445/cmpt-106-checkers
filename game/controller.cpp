#include <iostream>
#include <vector>
#include <array>
#include "HumanPlayer.h"
#include "AIPlayer.h"
#include "Display.h"


using namespace std;

struct Data{
    int turns;
    bool win;
    bool loss;
    bool tie;
    int piecesLost;
    Data(int turns, bool win, bool loss, bool tie, int piecesLost)
    :turns(turns),win(win),loss(loss),tie(tie),piecesLost(piecesLost)
    {}
};


struct CheckerController{
	//order of player parameters matter
	//board will treat the first player passed in as player 1
	//and have their pieces at the bottom
    array<Data,2> gameLoop(Player* player1, Player* player2){
        Display display;
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

            display.displayGame(board.gameBoard);
            cout << "Player 1's move" << endl;
            listOfPlayers[0]->findMoves(board.gameBoard);

            //canMove is updated inside getMove
            listOfPlayers[0]->getMove(board.gameBoard);
            if(!listOfPlayers[0]->canMove){break;}
            board.update(listOfPlayers,0);
            cout << "Player 1 move: " << listOfPlayers[0]->movesAsString.at(listOfPlayers[0]->indexOfMove) << endl;


            display.displayGame(board.gameBoard);
            cout << "Player 2's move" << endl;
            listOfPlayers[1]->findMoves(board.gameBoard);
            listOfPlayers[1]->getMove(board.gameBoard);
            if(!listOfPlayers[1]->canMove){break;}
            board.update(listOfPlayers,1);
            cout << "Player 2 move: " << listOfPlayers[1]->movesAsString.at(listOfPlayers[1]->indexOfMove) << endl;


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

        Data dataPlayer1(turns,listOfPlayers[0]->canMove,player1Loss,tie,player1LostPieces);
        Data dataPlayer2(turns,listOfPlayers[1]->canMove,player2Loss,tie,player2LostPieces);
        array<Data,2> arr = {dataPlayer1,dataPlayer2};
        return arr;
    }
};
