//
// Created by Jeremy S on 2017-11-19.
//

#pragma once

#include "base.hpp"
#include "clock.hpp"

// Config that can not be defined as variables.
#define NUM_ENTITY_PER_SET  100

#ifndef NUM_THREADS
#define NUM_THREADS 4
#endif

#ifndef NEURAL_IN
#define NEURAL_IN   2
#endif

#ifndef NEURAL_OUT
#define NEURAL_OUT  1
#endif


// Training return values
#define TRAIN_DONE  0
#define TRAIN_KILL  9
#define TRAIN_FIN   1

#define KILLSYS     8

namespace ml {

    // Config Singleton
    // This class holds all the settings for the system.
    // Values may be freely edited manualy or by the program
    // before or during training.
    //
    // As this is a singleton, access to the object must be
    // done through the static instance 'global'.
    struct cfg {
        static ptr<cfg> global;

        // difference settings
        double diff_coefficient = 0.1;

        // mutation settings
        bool mutate_best_network = false;
        size_t clone_num = 10;
        size_t tweak_num = 30;
		
		
        // display settings
        size_t output_interval = 1;

        // learning core settings
        bool use_goal = true;
        double goal = 0.015;
        size_t display_update_time = 250; // in milliseconds
        size_t save_interval = 2;
        std::string save_path = "net.txt";
        bool pause_on_convergence = false;
        size_t convergence_interval = 500;

        // cl settings
        bool ties_count_as_wins = false;
        size_t num_games = 5; // will end up playing 4x this number

        // comparator functions
        bool(*fit_val_comp)(double, double) = [](double e1, double e2) -> bool {
            return e1 < e2;
        };

    };

    ptr<cfg> cfg::global{new cfg{}};




    // Flag Singleton
    struct flags {
        static ptr<flags> global;

        bool kill_thread_exec = false;

        util::clock clock{};

    };

    ptr<flags> flags::global{new flags{}};

}
