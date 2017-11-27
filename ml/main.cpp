// This is the main training source file.

#include <iostream>
#include <ctime>

#define NEURAL_IN   65
#define NEURAL_OUT  32
#define NUM_THREADS 1

// #define LETS_PLAY_FIND_THE_SEGFAULT

#ifdef LETS_PLAY_FIND_THE_SEGFAULT
#define __debug(MSG) {printf("Debug - %s\n", (MSG)); fflush(stdout);}
#define __debug_l(L) {printf("Debug - %ld\n", (L)); fflush(stdout);}
#else
#define __debug(MSG)
#define __debug_l(L)
#endif

// #include "tournament.hpp"
#include "rlearning.hpp"

#define DISABLE_DISPLAY
#include "../game/controller.cpp"

int main() {
    srand((unsigned int)time(nullptr));

    ptr<cl::game_template<rl::ai_player>> game{new CheckerController{}};

    ptr<ml::network_o> seed_net{new network::NNetwork<NEURAL_IN, NEURAL_OUT>{std::vector<size_t>{60, 40}}};

    __debug("init")
    rl::learning_set trainer{seed_net, game};
    trainer.run_training(1000000000);
    return 0;

 //    ptr<cl::game_template<AIPlayer>> game{new CheckerController{}};

 //    // Load network from save path, or define a brand new one.
	// ptr<ml::network_o> seed_net{new network::NNetwork<NEURAL_IN, NEURAL_OUT>{ml::cfg::global->save_path}};
 //    // ptr<ml::network_o> seed_net{new network::NNetwork<NEURAL_IN, NEURAL_OUT>{std::vector<size_t>{60, 40}}};
	// // seed_net->save(ml::cfg::global->save_path);
 //    // return 0;

 //    cl::tournament_set training_set{seed_net, game};

 //    training_set.run_training(100000000); // run forever!!!
 //    training_set.save(); // save the final result
}