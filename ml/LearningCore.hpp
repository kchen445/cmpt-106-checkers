#pragma once

#include <array>
#include <tuple>
#include <memory>
#include <iostream>
#include <algorithm>
#include "Config.hpp"
#include "NetworkType.hpp"
#include "LearningEntity.hpp"
#include "LearningSet.hpp"
#include "LearningThread.hpp"
#include "Display.hpp"
#include "Flags.hpp"

#define KILLSYS 8

namespace ml {


    template<size_t In, size_t Out>
    using LSetRef = std::shared_ptr<LearningSet<In, Out, NUM_ENTITIES_PER_SET>>;

    template<size_t In, size_t Out>
    using LEntityRef = std::shared_ptr<LearningEntity<In, Out>>;

    template<size_t In, size_t Out>
    using LThreadRef = std::shared_ptr<LearningThread<In, Out, NUM_ENTITIES_PER_SET>>;


    // The object in charge of handling the learning threads which run
    // training sesions simultaiously. This object manages the threads
    // and periodically checks how they are doing. This check is refered
    // to as a convergence.
    //
    // During thread convergence, the best entity is copied and implanted
    // into the worst preforming thread.
    //
    // This object is not meant to be use on its own. It is meant to be
    // a parent object for subclasses which deal with different types
    // of learning.
    template<size_t In, size_t Out>
    class LearningCore {
    private:

        // Returns true if all threads in the threads array are inactive.
        bool allThreadsFinished () {
            bool finished = true;
            for (size_t i = 0; i < threads.count; ++i) {
                finished = finished && !(threads[i]->active);
            }
            return finished;
        }


        // Allows the user to run commands duing thread convergence.
        int pauseCommandInterface (LEntityRef<In, Out> bestEntity) {
            while (true) {

                try {

                    std::string command = "";
                    std::cout << "Command Interface [continue/quit/save/set <string> <int>]" << std::endl;
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
                        bestEntity->network->save(cfg->savePathForBestEntity);
                    }

                    else if (command == "set") {
                        // TODO: implement this
                    }

                    else {
                        std::cout << "Invalid command" << std::endl;
                    }

                } catch (...) {

                }
            }

            return 0;
        }

    protected:

        std::shared_ptr<Config> cfg = Config::global;

        // May be overriden by subclasses for extra implementation during convergence.
        virtual void onConvergence (LEntityRef<In, Out> bestEntity) {}

        // Used in picking the best thread.
        virtual bool threadStatsComparison (std::tuple<int, double> const &lhs, 
                                            std::tuple<int, double> const &rhs) 
        {
            return std::get<1>(lhs) < std::get<1>(rhs);
        }

    public:

        std::array<LSetRef<In, Out>, NUM_THREADS> learningSets;

        std::array<LThreadRef<In, Out>, NUM_THREADS> threads;


        // Default contructor
        // *** NOTE ***
        // The default constructor does not populate the arrays just
        // initalizes them. This means that simply calling the default
        // constructor does not make this object ready for use.
        //
        // Arrays should be populated in the constructors of subclasses.
        LearningCore ()
            : learningSets(),
              threads()
        {}


        // Runs a training routine.
        // To define and use a goal, do so in the Config singleton.
        // If no goal is defined training will run until terminated
        // by the user.
        std::shared_ptr<NetworkType<In, Out>> train () {

            double bestValue = 0.0;
            LEntityRef<In, Out> bestEntity = nullptr;
            size_t convergenceCount = 0;

            Display::instance->setup(NUM_THREADS);
            Flags::global->killThreadExecution = false;

            do {

                // Start thread execution.
                for (size_t i = 0; i < threads.size(); ++i) {
                    if (cfg->useGoal) {
                        threads[i]->start(cfg->goal);
                    } else {
                        threads[i]->start();
                    }
                }

                // Wait till the threads have finished.
                // Print out reports from Display.
                size_t count = 0;
                while (!allThreadsFinished()) {
                    if (count >= cfg->displayUpdateInterval) {
                        std::shared_ptr<std::string> report = Display::instance->getNextReport();
                        if (report != nullptr) {
                            std::cout << *report << std::endl;
                            count = 0;
                        }
                    } else {
                        ++count;
                    }
                }


                // -- Thread Convergence -- //

                // Get the value for the best network from each thread.
                //  Tuple form:
                //      (threadId: int, fitnessValue: double)
                std::array<std::tuple<int, double>, NUM_THREADS> threadStats{};
                for (size_t i = 0; i < threads.size(); ++i) {
                    std::get<0>(threadStats[i]) = (int)i;
                    std::get<1>(threadStats[i]) = threads[i]->learningSet->entities[0]->getValue();
                }
                std::sort(threadStats.begin(), threadStats.end(), this->threadStatsComparison);

                int bestThreadId = std::get<0>(threadStats.front());
                int worstThreadId = std::get<0>(threadStats.back());

                // Set bestEntity to be the best entity out of all the threads.
                bestEntity = threads[bestThreadId]->learningSet->entities[0];
                bestValue = bestEntity->getValue();

                // Clone the best network.
                std::shared_ptr<NetworkType<In, Out>> bestClone = bestEntity->network->clone();
                // Tweak it slightly.
                bestClone->tweakWeight(10, 0.1);
                // Insert it into the worst network.
                // Change the network of the worst entity to be the clone.
                threads[worstThreadId]->learningSet.back()->network = bestClone;

                // Do any extra stuff implemented by subclasses.
                onConvergence(bestEntity);

                // Autosave
                if (convergenceCount % cfg->saveInterval == 0) {
                    std::cout << "Saving..." << std::endl;
                    bestEntity->network->save(cfg->savePathForBestEntity);
                }


                // Display convergence data and start pause interface.
                std::cout << " -- Thread Convergence " << convergenceCount << " --"; 
                std::cout << std::endl << std::endl;

                // Get a forced report from display and print it out.
                std::shared_ptr<std::string> report = Display::instance->getNextReport(true);
                if (report != nullptr) {
                    std::cout << *report << std::endl;
                }

                // Print out the goal if there is one
                if (cfg->useGoal) {
                    std::cout << "Target: " << cfg->goal << std::endl;
                }
                std::cout << "Best  : " << bestEntity->getValue() << std::endl << std::endl;

                // Pause
                if (cfg->pauseOnConvergence) {
                    std::cout << "Paused" << std::endl;
                    if (pauseCommandInterface(bestEntity) == KILLSYS) {
                        return bestEntity->network;
                    }
                }


                ++convergenceCount;

            } while (!Flags::global->killThreadExecution);

        } // method train

    }; // class LearningCore

} // namespace ml
