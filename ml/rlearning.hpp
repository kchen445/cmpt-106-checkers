#pragma once

#include "framework/set_t.hpp"
#include "syscl.hpp"
#include "ai_player.hpp"

#include <cstdlib>
#include <cstdio>
#include <algorithm>
#include <array>
#include <fstream>
#include <iostream>

namespace rl {

    class learning_set : public ml::set_t<ai_player> {
    private:

        typedef ai_player                       entity_t;
        typedef cl::game_template<entity_t>     game_t;

        ptr<game_t> game;
        std::fstream data_out;

        std::array<ptr<entity_t>, 5> reference_entities{};

    public:

        learning_set (ptr<ml::network_o> const &seed, ptr<game_t> game)
                : ml::set_t<entity_t>(seed),
                  game(std::move(game)),
                  data_out()
        {
            data_out.open("learning_data.txt", std::fstream::app);
        }

        ~learning_set() override {
            data_out.close();
        }

        void save () {
            entities[0]->network->save(ml::cfg::global->save_path);
            entities[0]->network->save("networks/G" + std::to_string(step_count) + "P1.txt");
            entities[0]->network->save("networks/G" + std::to_string(step_count) + "P2.txt");
        }

        void sort_entities () override {
            std::sort(this->entities.begin(), this->entities.end(), [](auto e1, auto e2) -> bool {
                return e1->get_value() > e2->get_value();
            });
        }

        void before_exec () override {
            for (auto const &e : entities) {
                e->reset();
            }
        }

        void evaluate (size_t idx) override {
            for (auto const &ref_entity : reference_entities) {
                // first game
                entities[idx]->player = 1;
                ref_entity->player = 2;
                __debug("player id set ok")
                auto result_1 = game->compete(*entities[idx], *ref_entity);
                __debug("game ok")
                entities[idx]->process_results(result_1);
                __debug("process ok")
                // second game
                entities[idx]->player = 2;
                ref_entity->player = 1;
                auto result_2 = game->compete(*ref_entity, *entities[idx]);
                entities[idx]->process_results(result_2);
            }
        }

        void mutate () override {
            const double keep_lim = 0.4;
            const double tweak_lim = 0.6;
            const double rand_lim = 0.9;

            for (size_t i = 1; i < entities.size(); ++i) {
                if (i >= entities.size() * keep_lim && i < entities.size() * tweak_lim) {
                    entities[i]->network->tweakWeight(20, 0.1);
                    entities[1]->network->tweakBias(10, 0.05);
                } else if (i >= entities.size() * tweak_lim && i < entities.size() * rand_lim) {
                    entities[i]->network->tweakWeight(100, 1);
                    entities[i]->network->tweakBias(100, 1);
                } else {
                    entities[i]->network = entities[0]->network->clone();
                    entities[i]->network->tweakWeight(30, 0.2);
                    entities[1]->network->tweakBias(20, 0.1);
                }
            }
        }

        void run_training (size_t num_rounds, size_t save_interval = 10) {
            ml::display::interface.unsafe_raw()->setup();
            ml::flags::global->clock.set_start();

            __debug("training init")

            for (size_t i = 0; i < num_rounds; ++i) {
                if (step_count % save_interval == 0) {
                    save();
                }

                if (step_count % 1000 == 0) {
                    for (size_t idx = 0; idx < reference_entities.size(); ++idx) {
                        reference_entities[idx] = ptr<entity_t>{new entity_t{entities[idx]->network->clone()}};
                    }
                }

                __debug_l(entities.size())
                step();
                __debug("step")
                ml::display::interface.unsafe_raw()->print_report();

                // output to data file
                data_out << step_count << ' ';
                data_out << ml::flags::global->clock.elapsed().condence() << ' ';
                for (auto const &e : entities) {
                    data_out << e->get_value() << ' ';
                }
                data_out << std::endl;
            }
        }

    };

}
