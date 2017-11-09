#pragma once

#include <cstdlib>
#include <memory>
#include <string>


// preprocessor definitions (for templates)

#define NUM_ENTITIES_PER_SET 100
#define NUM_THREADS 4


// To avoid duplicate definitions of global variables and to avoid
// having to declare the variables as extern, I'll use a 
// singleton to define the varaibles.
//
// To access the variables use:
//      Config::global-><name>;
class Config {
public:

    // Pointer to the singleton instance.
    // As this is a shared pointer, it is ok to define
    // shared pointers to this object for ease of use.
    //
    //  e.g.
    //      std::shared_ptr<Config> cfg{Config::global};
    static std::shared_ptr<Config> global;

    double differenceCoef = 0.1;

    // Mutation settings
    bool mutateBestNetwork = false;
    size_t cloneAmount = 1;
    size_t smallChangeAmount = 5;

    // Display settings
    int outputInterval = 1;

    // Learning Core settings
    bool useGoal = false;
    double goal = 0.0;
    // how frequently the system queries display for a new report
    int displayUpdateInterval = 100; 
    int saveInterval = 2;
    std::string savePathForBestEntity = "trn_net.txt";
    bool pauseOnConvergence = true;

    // CLEntity settings
    bool tiesCountAsWins = false;

};

// Out of line initalization for static variable Config::global.
std::shared_ptr<Config> Config::global{new Config{}};
