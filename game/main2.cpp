#include "controller.hpp"

using namespace std;

int main(){
    CheckerController controller;
    //Player* player1 = new AIPlayer(1);
    Player* player1 = new HumanPlayer(1);
    //Player* player2 = new HumanPlayer(2);
    Player* player2 = new AIPlayer(2, "random_network.txt");
    controller.gameLoop(player1,player2);
}