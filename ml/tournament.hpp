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
			/*
			evolstats << "Time" << ' ';
			evolstats << "Generation" << ' ';
			evolstats << "Total_Turns" << ' ';
			evolstats << "Ties" << ' ';
			evolstats << "P1_Wins" << ' ';
			evolstats << "P2_Wins" << ' ';
			evolstats << "P1_LostPieces" << ' ';
			evolstats << "P2_LostPieces" << ' ';
			evolstats << "Ratings" << std::endl;
             */
		}
		
		
		/*tournament_set (ptr<ml::network_o> const &seed)
                : ml::set_t<AIPlayer>(seed)
        {
			evolstats.open("evolution_data.txt", std::fstream::app);
		}*/
		
		~tournament_set() override {
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
            entities[0]->network->save(ml::cfg::global->save_path);
			//entities[0]->network->save(ml::cfg::global->save_path + "G" + std::to_string(step_count) + "P1.txt");
        }
		
		void save_samplegame() {
			entities[0]->network->save("networks/G" + std::to_string(step_count) + "P1.txt");
            entities[1]->network->save("networks/G" + std::to_string(step_count) + "P2.txt");
		}
		
		/*void save_all () {
            entities[0]->network->save("net.txt");
			entities[0]->network->save(ml::cfg::global->save_path + "G" + std::to_string(step_count) + "P1.txt");
            entities[1]->network->save(ml::cfg::global->save_path + "G" + std::to_string(step_count) + "P2.txt");
        }*/

        std::string get_time () {
            return util::as_str(ml::flags::global->clock.elapsed());
        }

		void evaluate(size_t idx) override {}
		
        // Run through a single tournament.
        // Should return the best fitness value from the set.
        // This is used for determining with the target is
        // reached.
        //
        // If a target is not used then 0 may be returned.
		
		struct statistics {
			unsigned int totalturns = 0;
			unsigned int p1wins = 0;
			unsigned int p2wins = 0;
			unsigned int ties = 0;
			unsigned int p1pieces = 0;
			unsigned int p2pieces = 0;
		};
		
        double step () override {			
			std::random_shuffle(entities.begin(), entities.end());
			
			statistics stats;
			
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
						
						stats.totalturns += results[0].turns;
						stats.ties += results[0].tie;
						stats.p1wins += results[0].win;
						stats.p2wins += results[1].win;
						stats.p1pieces += results[0].piecesLost;
						stats.p2pieces += results[1].piecesLost;
						
						/*std::cout << "Entity " << p1 << " vs. Entity " << p2 << std::endl;
						std::cout << results[0].turns << ' '
							 << results[0].win   << ' '
							 << results[0].loss  << ' '
							 << results[0].tie   << ' '
							 << results[0].piecesLost << std::endl;
							std::cout << results[1].turns << ' '
							 << results[1].win   << ' '
							 << results[1].loss  << ' '
							 << results[1].tie   << ' '
							 << results[1].piecesLost << std::endl;*/
						//	assume result = 1   if win
						//		   result = 0.5 if tie
						//	       result = 0   if loss
						
						//expected probabilities always add up to 1
						double e = elo::expected(entities[p1]->rating, entities[p2]->rating);
						expected[p1-i] += e;
						expected[p2-i] += 1-e;
						scores[p1-i] += results[0].win ? (results[0].win) : (results[0].tie * 0.5);
						scores[p2-i] += results[1].win ? (results[1].win) : (results[0].tie * 0.5);
					}
				}
				
				//using the results of the tournament, update the elo rating
				for (size_t j=0; j<TOURNEY_SIZE; j++) {
					//std::cout << i+j << ':' << expected[j] << ' ' << scores[j] << std::endl;
//					std::cout << "Entity " << i+j << ": " << entities[i+j]->rating;
					entities[i+j]->rating = elo::update_raw(entities[i+j]->rating, expected[j], scores[j]);
//					std::cout << " -> " << entities[i+j]->rating << std::endl;
				}
				
				/*char c;
				std::cin >> c;*/
				
			}
			
			//sort for mutation
			sort_entities();
			
			//write evolution data
			evolstats << ml::flags::global->clock.elapsed().condence() << ' ';
			evolstats << step_count << ' ';
			
			//write stats
			evolstats << stats.totalturns << ' ';
			evolstats << stats.ties << ' ';
			evolstats << stats.p1wins << ' ';
			evolstats << stats.p2wins << ' ';
			evolstats << stats.p1pieces << ' ';
			evolstats << stats.p2pieces << ' ';
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
				entities[unlucky]->rating = entities[survivor]->rating;
				entities[unlucky]->network->tweakWeight(80, 0.1);
				entities[unlucky]->network->tweakBias(80, 0.1);
				entities[unlucky]->network->mutateNode(3);
				entities[unlucky]->network->mutateConnection(5);
			}

            double average = 0;
            for (auto const &entity : entities) {
                average += entity->get_value();
            }
            average = average / double(entities.size());

			ml::p_report report{
                    0,
                    (size_t)step_count,
                    entities[0]->get_value(),
                    average,
                    0,
                    0
            };

            send_report_to_display(report);
            print_display();
			
            return 0;
        }


        // The main training function.
        void run_training (size_t num_rounds, size_t save_interval = 10) {
            ml::display::interface.unsafe_raw()->setup();
			
			std::cout << "if starting with a fresh network, the elo rating is 100 and the generation is 0" << std::endl;
			std::cout << "otherwise, look in evolution_data for the latest entry; elo rating is the 9th value, and generation is the 2nd value" << std::endl;
			
			double rating;
			std::cout << "What is the elo rating of the seed network? ";
			std::cin >> rating;
			for (auto &e : entities) {
				e->rating = rating;
			}
			
			std::cout << "What generation was the seed network? ";
			std::cin >> step_count;
			step_count++;
			
			ml::flags::global->clock.set_start();
            for (size_t i = 0; i < num_rounds; ++i) {
                // Autosave in case of crash
                if (step_count % save_interval == 0) {	//save two networks for a sample game
                    save_samplegame();
                }
				save();									//save current best network

                step();
                step_count++;
				
//				char c;
//				std::cin >> c;
            }
        }

    };

}
