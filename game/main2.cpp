#include "controller.hpp"

using namespace std;

int main(){
    CheckerController controller;
    //Player* player1 = new AIPlayer(1);
    Player* player1 = new HumanPlayer(1);
    //Player* player2 = new HumanPlayer(2);
    Player* player2 = new AIPlayer(2, "../ml/net.txt");
    auto results = controller.gameLoop(player1,player2);
	
    cout << results[0].turns << ' '
         << results[0].win   << ' '
         << results[0].loss  << ' '
         << results[0].tie   << ' '
         << results[0].piecesLost << endl;
	    cout << results[1].turns << ' '
         << results[1].win   << ' '
         << results[1].loss  << ' '
         << results[1].tie   << ' '
         << results[1].piecesLost << endl;
	
	return 0;
}