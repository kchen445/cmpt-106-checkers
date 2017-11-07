#pragma once

#include <tuple>
#include <algorithm>
#include <memory>
#include "LearningEntity.hpp"
#include "Display.hpp"

namespace ml {

    // Abstract class that contains a set of learning entities.
    // Has training functions for training the entities.
    template<size_t In, size_t Out, size_t N>
    class LearningSet {
    protected:

        // Reference to the global config singleton.
        std::shared_ptr<Config> cfg{Config::global};

        // The number of times step() has been called.
        int stepId;

        // The previous best and average fitness value.
        //  tuple form:
        //      (best: double, average: double)
        std::tuple<double, double> lastStats;

    public:

        // The set of entities in this learning set.
        // As the type is complex, in english:
        //      size N array of unique pointers to learning entities.
        std::array<std::unique_ptr<LearningEntity<In, Out>>, N> entities;

        // The id number of the thread that containes this learning set.
        // If not called from a LearningThread, then 0.
        int callingThreadId;


        LearningSet ()
            : stepId(-1),
              lastStats(std::make_tuple(0.0, 0.0)),
              entities(),
              callingThreadId(0)
        {}

        virtual ~LearningSet () {}

        // TODO: Write constructor from config and seed.
        

        // Evaluates the entity at a given index.
        virtual void evaluate (size_t i) = 0;

        // Called before evaluating the entities.
        virtual void beforeEvaulation () {}

        // Sorts the entities array via the operator< defined in LearningEntity type.
        // May be overriden for specific functionality.
        virtual void sortEntities () {
            std::sort(entities.begin(), entities.end());
        }

        // Mutates each network depending on its ranking.
        void mutate () {
            size_t startIndex = cfg->mutateBestNetwork ? 0 : 1;
        
            for (size_t i = startIndex; i < entities.size(); ++i) {
                if (i > (entities.size() - 1 - cfg->cloneAmount)) {
                    entities[i]->network = entities[0]->network->clone();
                    entities[i]->network->tweakWeight(20, 0.1);
                    entities[i]->network->tweakBias(10, 0.1);
                } else if (i < cfg->smallChangeAmount) {
                    entities[i]->network->tweakWeight(60, 0.3);
                    entities[i]->network->tweakBias(40, 0.2);
                } else {
                    entities[i]->network->randomizeWeight(100);
                    entities[i]->network->ranomizeBias(100);
                }
            }
        }

        // Runs through a single training session.
        // Returns the value of the best preforming network.
        double step () {
            ++stepId;

            // Don't mutate until the networks have been evaluated at least once.
            if (stepId != 0) {
                mutate();
            }

            // Evaluate each network by calling the abstract evaulate() method.
            for (size_t i = 0; i < entities.size(); ++i) {
                this->evaulate(i);
            }

            // Sort the entities after evaulating them.
            sortEntities();

            // Send a progress report the display.
            double average = 0.0;
            for (size_t i = 0; i < entities.size(); ++i) {
                average += entities[i]->getValue();
            }
            average = average / double(entities.size());

            double best = entities[0]->getValue();

            ProgressReport report{
                callingThreadId,
                stepId,
                best,
                average,
                best - std::get<0>(lastStats),
                average - std::get<1>(lastStats)
            };

            Display::instance->addData(report);

            std::get<0>(lastStats) = best;
            std::get<1>(lastStats) = average;

            return best;
        }

    };

}
