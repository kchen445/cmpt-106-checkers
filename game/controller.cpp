#include "controller.hpp"

// #define DISABLE_DISPLAY before including this file for training system

using namespace std;

using Data = cl::game_data;


//order of player parameters matter
//board will treat the first player passed in as player 1
//and have their pieces at the bottom
inline Data CheckerController::gameLoop(Player* player1, Player* player2){
#ifndef DISABLE_DISPLAY
        Display display;
#endif
        Board board;
        
        Data stats;
        int turns = 0;

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
            char taken;

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
            taken = board.update(listOfPlayers,0);
            stats.p1take[turns/5] += taken;
            stats.p1taken += taken;
            
            
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
            taken = board.update(listOfPlayers,1);
            stats.p2take[turns/5] += taken;
            stats.p2taken += taken;
            
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
            
            /*char c;
            std::cin >> c;*/
        }
        

        if(!listOfPlayers[1]->canMove && listOfPlayers[1]->pieces.size() == 0){
#ifndef DISABLE_DISPLAY
            cout << endl << "Player 1 wins!" << endl << endl;
#endif
            stats.winner = 1;
            //player2Loss = true;
        }else if (!listOfPlayers[0]->canMove && listOfPlayers[0]->pieces.size() == 0){
#ifndef DISABLE_DISPLAY
            cout << endl << "Player 2 wins!" << endl << endl;
#endif
            stats.winner = 2;
            //player1Loss = true;
        }else{
            stats.tie = true;
            if(stats.p1taken > stats.p2taken){
                stats.winner = 0; // was 1
                //listOfPlayers[1]->canMove = false;
                //player2Loss = true;
#ifndef DISABLE_DISPLAY
                cout << "Stalemate with Player 1 taking more pieces!" << endl;
#endif
            }else if(stats.p1taken == stats.p2taken){
                stats.winner = 0;
                //listOfPlayers[0]->canMove = false;
                //listOfPlayers[1]->canMove = false;
#ifndef DISABLE_DISPLAY
                cout << "Complete Tie" << endl;
#endif
            }else{
                stats.winner = 0; // was 2
                //listOfPlayers[0]->canMove = false;
                //player1Loss = true;
#ifndef DISABLE_DISPLAY
                cout << "Stalemate with Player 2 taking more pieces!" << endl;
#endif
            }
        }
        
        stats.turns = turns;
        return stats;
}
    
    
// Override from cl::game_template<AIPlayer>
inline Data CheckerController::compete (rl::ai_player &e1, rl::ai_player &e2) {
    e1.player = 1;
    e2.player = 2;
    return gameLoop(&e1, &e2);
}
