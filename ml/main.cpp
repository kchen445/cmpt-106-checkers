// This is the main training source file.

#include <iostream>
#include <ctime>

#define NEURAL_IN   64
#define NEURAL_OUT  32
#define NUM_THREADS 1

#include "tournament.hpp"

#define DISABLE_DISPLAY
#include "../game/controller.cpp"

int main() {
    srand((unsigned int)time(nullptr));

    ptr<cl::game_template<AIPlayer>> game{new CheckerController{}};

    // Load network from save path, or define a brand new one.
	
	ptr<ml::network_o> seed_net{new network::NNetwork<NEURAL_IN, NEURAL_OUT>{ml::cfg::global->save_path}};
	cl::tournament_set training_set{seed_net, game};
	
    /*try {		//try loading the backup
		ptr<ml::network_o> seed_net{new network::NNetwork<NEURAL_IN, NEURAL_OUT>{ml::cfg::global->save_path}};
		training_set{seed_net, game};
	} catch (const std::runtime_error& e) {
		training_set{game};
		
	}*/
    

    training_set.run_training(100000000); // run forever!!!
    training_set.save(); // save the final resultptr<cl::game_template<AIPlayer>> game{new CheckerController{}};

    // Load network from save path, or define a brand new one.
    //ptr<ml::network_o> seed_net{new network::NNetwork<NEURAL_IN, NEURAL_OUT>{ml::cfg::global->save_path}};
    /*ptr<ml::network_o> seed_net{new network::NNetwork<NEURAL_IN, NEURAL_OUT>(std::vector<size_t>(0))};

    cl::tournament_set training_set{seed_net};

    training_set.run_training(1); // run 1000 generations
    training_set.save(); // save the final result*/
}