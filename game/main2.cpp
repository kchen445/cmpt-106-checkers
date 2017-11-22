#include "controller.hpp"
#include <memory>

using namespace std;

void play_against_old (size_t num_games) {
    CheckerController controller;
    std::unique_ptr<Player> p2{new AIPlayer{2, "../bin/net.txt"}};
    for (size_t i = 0; i < num_games; ++i) {
        std::unique_ptr<Player> p1{new AIPlayer{1, "../bin/networks/G" + std::to_string(10 * (i + 1)) + "P1.txt"}};
        auto results = controller.gameLoop(p1.get(), p2.get());
        
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
        
        std::cout << "Press enter to play next game...";
        std::string s;
        std::getline(std::cin, s);
    }
}

int main(int argc, char **argv){
    std::cout << argc << std::endl;
    if (argc == 2 && std::string(argv[1]) == "-c") {
        play_against_old(100);
    } else {
        CheckerController controller;
        //Player* player1 = new AIPlayer(1);
        Player* player1 = new AIPlayer{1, "../bin/networks/G10P2.txt"};
        //Player* player2 = new HumanPlayer(2);
        Player* player2 = new AIPlayer(2, "../bin/net.txt");
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
    }
	return 0;
}
