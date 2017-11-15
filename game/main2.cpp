#include "controller.cpp"

using namespace std;

int main(){
    CheckerController controller;
    Player* player1 = new HumanPlayer(1);
    Player* player2 = new HumanPlayer(2);
    controller.gameLoop(player1,player2);
}