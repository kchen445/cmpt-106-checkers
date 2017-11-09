#pragma once

#include "../MLFramework.hpp"

namespace ml {

    template<size_t In, size_t Out>
    class CLEntity : public LearningEntity<In, Out> {
    public:

        size_t wins = 0;
        size_t losses = 0;
        size_t ties = 0;

        CLEntity (std::shared_ptr<NetworkType<In, Out>> network)
            : LearningEntity<In, Out>(network)
        {}

        // Returns the total number of games played by this entity.
        size_t numberGamesPlayed () const {
            return wins + losses + ties;
        }

        // Returns the win rate for this entity.
        double getValue () const override {
            if (Config::global->tiesCountAsWins) {
                return double(wins + ties) / double(numberGamesPlayed());
            } else {
                return double(wins) / double(numberGamesPlayed());
            }
        }

        // Resets all the stats in this entity.
        void reset () override {
            wins = 0;
            losses = 0;
            ties = 0;
        }

        // Assuming other is also a CLEntity.
        bool operator< (LearningEntity<In, Out> const &other) override {
            return this->getValue() > other.getValue();
        }

    };

}
