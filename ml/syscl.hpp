//
// Created by Jeremy S on 2017-11-14.
//

#pragma once

#include <tuple>
#include <array>
#include "sysml.hpp"

namespace cl {

    // replace with core mechanics data struct
//    using data = <>
    struct data {
        size_t win;
        size_t loss;
        size_t ties;
        size_t num_lost;
        size_t game_length;
    };


    // AIPlayer should extend this class.
    struct entity_trad : public ml::entity_t {

        double fit_val = 0;

        explicit entity_trad (ptr<ml::network_o> const &n)
                : ml::entity_t(n)
        {}

        // p_idx is 1 if player is the first parameter in the
        // compete function, 2 if it is the second.
        virtual void pre_game_setup (int p_idx) {}

        double get_value () const override {
            return fit_val;
        }

        // Compute the fitness value here and store it in fit_val.
        // Does not need to be overriden, will probably be done in
        // this class.
        virtual void process_game_data (data const &d) {}
    };


    // GameController should extend this class
    class game_template {
    public:

        virtual std::array<data, 2> compete (ml::entity_t const &e1, ml::entity_t const &e2) = 0;

    };


    class set_trad : ml::set_t<entity_trad> {
    private:

        // Stride around the entities array.
        size_t circular_stride (size_t start, size_t length) {
            return start + length >= this->entities.size() ? start + length - entities.size() : start + length;
        }

    public:

        ptr<game_template> game;

        set_trad (ptr<ml::network_o> const &seed, ptr<game_template> game)
                : ml::set_t<entity_trad>(seed),
                  game(std::move(game))
        {}


        void before_exec () override {
            std::random_shuffle(this->entities.begin(), this->entities.end());
        }


        void evaluate (size_t idx) override {
            for (size_t s = 0; s < config->num_games; ++s) {
                size_t other_idx = circular_stride(idx, s);

                entities[idx]->pre_game_setup(1);
                entities[other_idx]->pre_game_setup(2);
                auto game_data = game->compete(*entities[idx], *entities[other_idx]);
                entities[idx]->process_game_data(game_data[0]);
                entities[other_idx]->process_game_data(game_data[1]);

                // swap sides and play again
                entities[idx]->pre_game_setup(2);
                entities[other_idx]->pre_game_setup(1);
                game_data = game->compete(*entities[other_idx], *entities[idx]);
                entities[idx]->process_game_data(game_data[1]);
                entities[other_idx]->process_game_data(game_data[0]);

            }
        }

    };

}
