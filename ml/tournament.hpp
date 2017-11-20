#pragma once

#include "../ai/AIPlayer.h"
#include "sysml.hpp"
#include "syscl.hpp"

namespace cl {

    class tournament_set : protected ml::set_t<AIPlayer> {
    public:
        /*
         * From ml::set_t:
         *
         * std::shared_ptr<ml::cfg> config;         // reference to config singleton
         * int step_count;                          // used to count the number of times step() has been called
         * std::tuple<double, double> last_stats;   // used for display
         * std::array<std::shared_ptr<AIPlayer>, NUM_ENTITIES_PER_SET> entities;
         * size_t calling_thread_id;                // for multithreading
         */

        typedef AIPlayer entity_t;

        // game->compete(AIPlayer const &e1, AIPlayer const &e2) is used to play a game
        // index 0 of output array coresponds to the player data for e1, index 1 for e2
        // see syscl.hpp for player_data struct definition
        ptr<game_template<entity_t>> game; // the game object using to evaluate the entities


        // Populate this set via a seed network.
        // Calls explicit network constructor for entity type.
        tournament_set (ptr<ml::network_o> const &seed, ptr<game_template> game)
                : ml::set_t<AIPlayer>(seed),
                  game(std::move(game))
        {}


        // Function to sort the entities using whatever method.
        void sort_entities () override {

        }


        // Send a progress report to the display.
        // See framework/display.hpp for p_report struct.
        void send_report_to_display (ml::p_report const &report) {
            ml::display::interface.unsafe_raw()->add(report);
        }


        // Tells the display to print.
        void print_display () {
            ml::display::interface.unsafe_raw()->print_report();
        }


        // Saves the best network.
        // The file path of where to save is defined in the config singleton
        // and may be set using ml::cfg::global->save_path = "<path>";
        void save () {
            entities[0]->network->save(config->save_path);
        }


        // Run through a single tournament.
        // Should return the best fitness value from the set.
        // This is used for determining with the target is
        // reached.
        //
        // If a target is not used then 0 may be returned.
        double step () override {

            return 0;
        }


        // The main training function.
        void run_training (size_t num_rounds, size_t save_interval = 10) {
            for (size_t i = 0; i < num_rounds; ++i) {
                // Autosave in case of crash
                if (num_rounds % save_interval == 0) {
                    save();
                }

                step();
                step_count++;
            }
        }

    };

}
