#pragma once

#include "../ai/elo.hpp"

#include "../game/AIPlayer.h"
#include "sysml.hpp"
#include "syscl.hpp"

#include <algorithm>
#include <array>
#include <fstream>
#include <iostream>

#define TOURNEY_SIZE 5
		
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

		std::fstream evolstats;
		
        // Populate this set via a seed network.
        // Calls explicit network constructor for entity type.
        tournament_set (ptr<ml::network_o> const &seed, ptr<game_template<entity_t>> game)
                : ml::set_t<AIPlayer>(seed),
                  game(std::move(game))
        {
			evolstats.open("evolution_data.txt", std::fstream::app);
		}
		
		
		/*tournament_set (ptr<ml::network_o> const &seed)
                : ml::set_t<AIPlayer>(seed)
        {
			evolstats.open("evolution_data.txt", std::fstream::app);
		}*/
		
		~tournament_set() {
			evolstats.close();
		}

        // Function to sort the entities in order of descending rating.
        void sort_entities () override {
			std::sort(entities.begin(), entities.end(), [](std::shared_ptr<AIPlayer> p1, std::shared_ptr<AIPlayer> p2) { return p1->rating > p2->rating; });
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

		void evaluate(size_t idx) 
		{}
		
        // Run through a single tournament.
        // Should return the best fitness value from the set.
        // This is used for determining with the target is
        // reached.
        //
        // If a target is not used then 0 may be returned.
        double step () override {			
			std::random_shuffle(entities.begin(), entities.end());
			
			
			
			//write evolution data
			evolstats << step_count << ' ';
			for (auto e : entities) {
				evolstats << e->rating << ' ';
			}
			evolstats << std::endl;		//should flush stream
			
			//have each group of TOURNEY_SIZE entities play a full round-robin tournament
			for (size_t i=0; i<entities.size(); i+=TOURNEY_SIZE)
			{
				std::array<double, TOURNEY_SIZE> expected{0};	//what score each player is expected to get
				std::array<double, TOURNEY_SIZE> scores{0};		//what score each player actually gets
				for (size_t p1=i; p1<i+TOURNEY_SIZE; p1++) {
					for (size_t p2=i; p2<i+TOURNEY_SIZE; p2++) {
						if (p1==p2) continue;
						
						//play the game
						std::array<player_data, 2> results = game->compete(*(entities[p1]), *(entities[p2]));
						
						std::cout << "Entity " << p1 << " vs. Entity " << p2 << std::endl;
						std::cout << results[0].turns << ' '
							 << results[0].win   << ' '
							 << results[0].loss  << ' '
							 << results[0].tie   << ' '
							 << results[0].piecesLost << std::endl;
							std::cout << results[1].turns << ' '
							 << results[1].win   << ' '
							 << results[1].loss  << ' '
							 << results[1].tie   << ' '
							 << results[1].piecesLost << std::endl;
						//	assume result = 1   if win
						//		   result = 0.5 if tie
						//	       result = 0   if loss
						
						//expected probabilities always add up to 1
						double e = elo::expected(entities[p1]->rating, entities[p2]->rating);
						expected[p1-i] += e;
						expected[p2-i] += 1-e;
						scores[p1-i] += results[0].win + results[0].tie * 0.5;
						scores[p2-i] += results[1].win + results[1].tie * 0.5;
					}
				}
				
				//using the results of the tournament, update the elo rating
				for (size_t j=0; j<TOURNEY_SIZE; j++) {
					//std::cout << i+j << ':' << expected[j] << ' ' << scores[j] << std::endl;
					std::cout << "Entity " << i << ": " << entities[i+j]->rating;
					entities[i+j]->rating = elo::update_raw(entities[i+j]->rating, expected[j], scores[j]);
					std::cout << " -> " << entities[i+j]->rating << std::endl;
				}
				
				char c;
				std::cin >> c;
				
			}
			
			//sort for mutation
			sort_entities();
			
			//write evolution data
			evolstats << step_count << ' ';
			for (auto e : entities) {
				evolstats << e->rating << ' ';
			}
			evolstats << std::endl;		//should flush stream
			
			//kill and mutate
			//ripped from https://www.youtube.com/watch?v=GOFws_hhZs8
			for (size_t i=0; i<entities.size()/2; i++) {
				double thresh = (double)i / entities.size();
				double rand = (std::pow(randDouble(-1,1),3)+1)/2;
				bool chance = (thresh <= rand);
				
				size_t survivor,unlucky;
				if (chance) {
					survivor = i;
					unlucky = entities.size()-1-i;
				} else {
					survivor = entities.size()-1-i;
					unlucky = i;
				}
				//std::cout << "deleting" << unlucky << std::endl;
				entities[unlucky]->network = entities[survivor]->network->clone();
				entities[unlucky]->network->tweakWeight(80, 0.1);
				entities[unlucky]->network->tweakBias(80, 0.1);
				entities[unlucky]->network->mutateNode(3);
				entities[unlucky]->network->mutateConnection(5);
			}
			
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
