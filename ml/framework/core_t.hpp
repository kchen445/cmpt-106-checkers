//
// Created by Jeremy S on 2017-11-19.
//

#pragma once

#include "set_t.hpp"
#include "l_thread.hpp"

namespace ml {

    // Class in charge of coordinating the threads
    template<typename E>
    class core_t {
    private:

        bool all_threads_finished () {
            bool done = true;
            for (auto t : threads) {
                done = done && !t->active;
            }
            return done;
        }

        int pause_interface (ptr<E> const &best_entity) {
            while (true) {

                try {

                    std::string command;
                    std::cout << "Command Interface [continue/quit/save]" << std::endl;
                    std::cout << " > ";
                    std::cin >> command;

                    // Un-pause and continue with learning.
                    if (command == "continue") {
                        break;
                    }

                        // Tell the learing core to stop.
                    else if (command == "quit") {
                        std::cout << "Terminating learning..." << std::endl;
                        return KILLSYS;
                    }

                        // Save the best network.
                    else if (command == "save") {
                        std::cout << "Saving..." << std::endl;
                        best_entity->network->save(cfg::global->save_path);
                    }

                    else if (command == "set") {
                        // TODO: implement this
                    }

                    else {
                        std::cout << "Invalid command" << std::endl;
                    }

                } catch (...) {
                    std::cout << "Unable to execute command." << std::endl;
                }
            }

            return 0;
        }

    protected:

        lang::tsafe_ptr<display> display_ptr{display::interface};

        virtual void on_convergence (ptr<E> const &best_entity) {};

        virtual void sort_stats (std::array<std::tuple<int, double>, NUM_THREADS> &stats) {
            std::sort(stats.begin(), stats.end(), [](auto e1, auto e2) -> bool {
                return std::get<double>(e1) < std::get<double>(e2);
            });
        }

    public:

        std::array<ptr<set_t<E>>, NUM_THREADS> l_sets;

        std::array<ptr<l_thread<E>>, NUM_THREADS> threads;

        // Default contructor
        // *** NOTE ***
        // The default constructor does not populate the arrays just
        // initalizes them. This means that simply calling the default
        // constructor does not make this object ready for use.
        //
        // Arrays should be populated in the constructors of subclasses.
        core_t ()
                : l_sets(),
                  threads()
        {}

        virtual ~core_t () = default;


        // Runs a training routine.
        // To define and use a goal, do so in the Config singleton.
        // If no goal is defined training will run until terminated
        // by the user.
        ptr<E> train () {

            // Setup
            double best_val = 0;
            ptr<E> best_entity = nullptr;
            size_t convergence_count = 0;

            display_ptr.unsafe_raw()->setup();
            flags::global->kill_thread_exec = false;
            flags::global->clock.set_start();


            // Training loop
            do {

                // Start the threads
                for (auto t : threads) {
                    t->start();
                }

                pthread_join(threads[0]->thread, NULL);

                // Wait till the threads have finished.
                // Periodically print out reports from display.
//                while (!all_threads_finished()) {
//                    // As to not be constantly querying display to print new data
//                    // we'll put the main thread to sleep for a set amount of time.
//                    std::chrono::milliseconds sleep_time{cfg::global->display_update_time};
//                    std::this_thread::sleep_for(sleep_time);
//                    display_ptr.exec([](display &src) {
//                        src.print_report();
//                    });
//                }

                // -- all threads have stopped here -- //

                // Thread Convergence

                // Get the best entity from each thread.
                //  tuple form:
                //      (thread_id: int, fitness_value: double)
                std::array<std::tuple<int, double>, NUM_THREADS> t_stats;
                for (size_t i = 0; i < threads.size(); ++i) {
                    std::get<int>(t_stats[i]) = (int)i;
                    std::get<double>(t_stats[i]) = threads[i]->l_set->entities[0]->get_value();
                }
                sort_stats(t_stats);

                int best_t_id = std::get<int>(t_stats.front());
                int worst_t_id = std::get<int>(t_stats.back());

                // Set best entity to be the best entity from all the threads.
                best_entity = threads[best_t_id]->l_set->entities[0];
                best_val = best_entity->get_value();

                // Clone the best network and tweak it slightly.
                ptr<E> clone{new E{best_entity->network->clone()}};
                clone->network->tweakWeight(10, 0.1);

                // Insert the clone into the worst thread.
                threads[worst_t_id]->l_set->entities.back() = clone;

                // Do any extra stuff
                on_convergence(best_entity);

                // Auto save
                if (convergence_count % cfg::global->save_interval == 0) {\
                    std::cout << "Saving ..." << std::endl;
                    best_entity->network->save(cfg::global->save_path);
                }


                // Display convergence info and pause interface
                std::cout << " -- Convergence - " << convergence_count << " -- " << std::endl << std::endl;
                display_ptr.unsafe_raw()->print_report();

                // Print out the goal if there is one
                if (cfg::global->use_goal) {
                    std::cout << "Target: " << cfg::global->goal << std::endl;
                }
                std::cout << "Best  : " << best_entity->get_value() << std::endl << std::endl;

                // Pause interface
                if (cfg::global->pause_on_convergence && !flags::global->kill_thread_exec) {
                    std::cout << "Paused" << std::endl;
                    if (pause_interface(best_entity) == KILLSYS) {
                        return best_entity;
                    }
                }

                ++convergence_count;

            } while (!flags::global->kill_thread_exec);

            std::cout << "Training Complete!" << std::endl;

            std::cout << "Saving ..." << std::endl;
            best_entity->network->save(cfg::global->save_path);

            return best_entity;

        }

    }; // class core_t

}
