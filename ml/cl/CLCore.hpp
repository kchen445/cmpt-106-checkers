#pragma once

#include "../MLFramework.hpp"
#include "CLSet.hpp"

namespace ml {

    // Player must conform to CLEntity<In, Out> and be constructable
    // from std::shared_ptr<NetworkType<In, Out>>
    template<class Player, size_t In, size_t Out>
    class CLCore : public LearningCore<CLEntity<In, Out>> {
    public:

        // The player that all other players play against.
        std::shared_ptr<Player> referencePlayer = nullptr;


        CLCore (std::shared_ptr<NetworkType<In, Out>> seed)
            : LearningCore<CLEntity<In, Out>>()
        {
            for (size_t i = 0; i < this->learningSets.size(); ++i) {
                this->learningSets[i] = CLSet<Player, In, Out>{
                    new CLGameTemplate<Player>{}, 
                    this->referencePlayer,
                    seed
                };
            }

            // Set the seed to be the referece player.
            referencePlayer = new Player{seed};
        }

        // TODO: write constructor from config

        // Comparison via > instead of <
        bool threadStatsComparison (std::tuple<int, double> const &lhs,
                                    std::tuple<int, double> const &rhs) override
        {
            return std::get<1>(lhs) > std::get<1>(rhs);
        }

        // Set the reference player to be a clone of the best player.
        // Reset the stats of all the players.
        void onConvergence (std::shared_ptr<CLEntity<In, Out>> bestEntity) override {
            referencePlayer = Player{bestEntity->network->clone()};
            for (size_t i = 0; i < this->learningSets.size(); ++i) {
                for (size_t j = 0; j < this->learningSets[i].entities.size(); ++j) {
                    this->learningSets[i]->entities[j]->reset();
                }
            }
        }

    };

}
