// This is the main source file for testing the ai and reviewing the training.

#define NEURAL_IN  65
#define NEURAL_OUT 32

#include "controller.hpp"
#include <memory>

using namespace std;

void print_results(const cl::game_data &results) {
	cout << results.turns << ' '
        << results.winner   << ' '
        << results.tie  << " | ";
	
	for (unsigned int i=0; i<results.p1take.size(); i++) {
		cout << (int)results.p1take[i] << " ";
	}
	cout << "| ";
	
	for (unsigned int i=0; i<results.p1take.size(); i++) {
		cout << (int)results.p2take[i] << " ";
	}
	cout << endl;
}


void play_against_old (size_t num_games) {
    CheckerController controller;
    std::unique_ptr<Player> p2{new AIPlayer{2, "../bin/net.txt"}};
    for (size_t i = 0; i < num_games; ++i) {
        std::unique_ptr<Player> p1{new AIPlayer{1, "../bin/networks/G" + std::to_string(10 * (i + 1)) + "P1.txt"}};
        auto results = controller.gameLoop(p1.get(), p2.get());
        
        print_results(results);
        
        std::cout << "Press enter to play next game...";
        std::string s;
        std::getline(std::cin, s);
    }
}

void replay (size_t num_games) {
    CheckerController controller;
    for (size_t i = 0; i < num_games; ++i) {
        std::unique_ptr<Player> p1{new AIPlayer{1, "../bin/networks/G" + std::to_string(10 * (i + 1)) + "P1.txt"}};
        std::unique_ptr<Player> p2{new AIPlayer{2, "../bin/networks/G" + std::to_string(10 * (i + 1)) + "P2.txt"}};
        auto results = controller.gameLoop(p1.get(), p2.get());
        print_results(results);
        std::cout << "Press enter to play next game, enter 'quit' to quit: ";
        std::string s;
        std::getline(std::cin, s);
        if (s == "quit") {
            exit(0);
        }
    }
}

int main(int argc, char **argv){
    std::cout << argc << std::endl;
    if (argc == 2 && std::string(argv[1]) == "-c") {
        play_against_old(100);
    } else if (argc == 2 && std::string(argv[1]) == "-r") {
        replay(100000);
    } else {
        CheckerController controller;
        //Player* player1 = new AIPlayer(1);
        Player* player1 = new AIPlayer{1, "../bin/networks/G1290P1.txt"};
        //Player* player2 = new HumanPlayer(2);
        //Player* player2 = new AIPlayer(2, "../bin/net.txt");
        Player* player2 = new AIPlayer(2, "../bin/networks/G1290P2.txt");
        auto results = controller.gameLoop(player1,player2);
        
		print_results(results);
    }
	return 0;
}
