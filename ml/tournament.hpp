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
            std::sort(entities.begin(), entities.end(), [](std::shared_ptr<AIPlayer> p1, std::shared_ptr<AIPlayer> p2) { return p1->performance > p2->performance; });
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
            unsigned int nontieturns = 0;
            unsigned int p1wins = 0;
            unsigned int p2wins = 0;
            unsigned int ties = 0;
            std::array<unsigned int, 15> p1take{};  //pieces taken by p1 in 5-turn timeslices
            std::array<unsigned int, 15> p2take{};  //pieces taken by p2
        };
        
        double step () override {           
            std::random_shuffle(entities.begin(), entities.end());
            
            statistics stats;

            
            
            //have each group of TOURNEY_SIZE entities play a full round-robin tournament
            for (size_t i=0; i<entities.size(); i+=TOURNEY_SIZE)
            {
                //values tracked for elo rating
                //  0 - lose, 1 - win
                std::array<double, TOURNEY_SIZE> expected{};        //what score each player is expected to get
                std::array<double, TOURNEY_SIZE> scores{};          //what score each player actually gets
                
                //performance in tournament. used to deter stalemates
                //  -1 - tie, 0 - lose, 1 - win
                std::array<double, TOURNEY_SIZE> performance{};
                
                //play tournament
                for (size_t p1=i; p1<i+TOURNEY_SIZE; p1++) {
                    for (size_t p2=i; p2<i+TOURNEY_SIZE; p2++) {
                        if (p1==p2) continue;
                        //play the game
                        game_data results = game->compete(*(entities[p1]), *(entities[p2]));
                        
                        //collect stats
                        stats.ties += results.tie;
                        stats.p1wins += (results.winner == 1);
                        stats.p2wins += (results.winner == 2);
                        
                        stats.totalturns += results.turns;
                        stats.nontieturns += (results.tie) ? 0 : results.turns;
                        
                        for (unsigned int j=0; j<results.p1take.size(); j++) {
                            stats.p1take[j] += results.p1take[j];
                            stats.p2take[j] += results.p2take[j];
                        }
                        
                        //adjust scores and performance
                        if (results.tie) {
                            performance[p1-i] -= 1;
                            performance[p2-i] -= 1;
                        } else {
                            double e = elo::expected(entities[p1]->rating, entities[p2]->rating);
                            expected[p1-i] += e;
                            expected[p2-i] += 1-e;
                            scores[p1-i] += (results.winner == 1);
                            scores[p2-i] += (results.winner == 2);
                            performance[p1-i] += (results.winner == 1);
                            performance[p2-i] += (results.winner == 2);
                        }
                    }
                }
                
                //using the results of the tournament, update the elo rating
                for (size_t j=0; j<TOURNEY_SIZE; j++) {
                    //std::cout << i+j << ':' << expected[j] << ' ' << scores[j] << std::endl;
//                  std::cout << "Entity " << i+j << ": " << entities[i+j]->rating;
                    entities[i+j]->rating = elo::update_raw(entities[i+j]->rating, expected[j], scores[j]);
                    entities[i+j]->performance = performance[j];
//                  std::cout << " -> " << entities[i+j]->rating << std::endl;
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
            evolstats << stats.ties << ' ';
            evolstats << stats.p1wins << ' ';
            evolstats << stats.p2wins << ' ';
            
            evolstats << stats.totalturns << ' ';
            evolstats << stats.nontieturns << ' ';
            
            for (unsigned int j=0; j<stats.p1take.size(); j++) {
                evolstats << stats.p1take[j] << ' ';
            }
            for (unsigned int j=0; j<stats.p1take.size(); j++) {
                evolstats << stats.p2take[j] << ' ';
            }
            
            for (auto e : entities) {
                evolstats << e->rating << ' ';
            }
            evolstats << std::endl;     //should flush stream
            
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
                
                entities[unlucky]->network = entities[survivor]->network->clone();
                entities[unlucky]->rating = entities[survivor]->rating;
                entities[unlucky]->network->tweakWeight(60, 0.2);
                entities[unlucky]->network->tweakBias(50, 0.1);
                entities[unlucky]->network->mutateNode(3);
                entities[unlucky]->network->mutateConnection(5);
            }

            double average = 0;
            double average_perf = 0;
            for (auto const &entity : entities) {
                average += entity->get_value();
                average_perf += entity->performance;
            }
            average = average / double(entities.size());
            average_perf = average_perf / double(entities.size());

            ml::p_report report{
                    0,
                    (size_t)step_count,
                    entities[0]->get_value(),
                    average,
                    entities[0]->performance,
                    average_perf
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
                if (step_count % save_interval == 0) {  //save two networks for a sample game
                    save_samplegame();
                }
                save();                                 //save current best network

                step();
                step_count++;
                
//              char c;
//              std::cin >> c;
            }
        }

    };

}
