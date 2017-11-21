//
// Created by Jeremy S on 2017-11-19.
//

#pragma once

#include "display.hpp"
#include "entity_t.hpp"

namespace ml {

    // Template type E must extend entity_t and be non-abstract.
    // Entity must also be constructable from ptr<network_o> i.e.
    //      std::shared_ptr<ml::network_o>
    template<typename E>
    class set_t {
    protected:
        ptr<cfg> config = cfg::global;

        int step_count;

        std::tuple<double, double> last_stats;

    public:

        std::array<ptr<E>, NUM_ENTITY_PER_SET> entities;

        size_t calling_thread_id;

        set_t ()
                : step_count(-1),
                  last_stats(std::make_tuple(0.0,0.0)),
                  entities(),
                  calling_thread_id(0)
        {}

        explicit set_t (ptr<network_o> const &seed)
                : step_count(-1),
                  last_stats(std::make_tuple(0.0, 0.0)),
                  entities(),
                  calling_thread_id(0)
        {
            for (auto &entity : this->entities) {
                auto seed_variant = seed->clone();
                seed_variant->tweakWeight(10, 0.1);
				entity = ptr<E>(new E(seed_variant));
                //entity = ptr<E>{new E{seed_variant}};
            }
        }

        virtual ~set_t () = default;

        // Populates this set with entities based off of a given seed.
        // This set should be empty before calling this method.
        //
        // Alternatively, the seed constructor may be used.
        void populate (ptr<network_o> const &seed) {
            for (auto &entity : this->entities) {
                auto seed_variant = seed->clone();
                seed_variant->tweakWeight(10, 0.1);
                entity = ptr<E>(new E(seed_variant));
				//ptr<E>{new E{seed_variant}};
            }
        }

        // Evaluates the entity at a given index.
        // The results of this evaluation should be stored in the entity.
        virtual void evaluate (size_t idx) = 0;

        // Called before evaluating the entities.
        virtual void before_exec () {}

        // Sort the entities usiing the operator < overload in the entity_t class.
        virtual void sort_entities () {
            std::sort(this->entities.begin(), this->entities.end(), [](ptr<E> e1, ptr<E> e2) -> bool {
                if (e1 == nullptr || e2 == nullptr) {
                    throw std::runtime_error{"bad sort"};
                }
                return e1->get_value() < e2->get_value();
            });
        }

        // Loops through all entities mutating them based on their position in the array.
        // The entities array should be sorted before this method is called.
        virtual void mutate () {
            size_t start_id = config->mutate_best_network ? 0 : 1;
            for (size_t i = start_id; i < this->entities.size(); ++i) {
                if (i > (entities.size() - 1 - config->clone_num)) {
                    this->entities[i]->network = entities[0]->network->clone();
                    this->entities[i]->network->tweakWeight(10, 0.05);
                    this->entities[i]->network->tweakBias(5, 0.025);
                } else if (i < config->tweak_num) {
                    this->entities[i]->network->tweakWeight(60, 0.3);
                    this->entities[i]->network->tweakBias(40, 0.2);
                } else {
                    this->entities[i]->network->randomizeWeight(100);
                    this->entities[i]->network->randomizeBias(100);
                }
            }
        }


        virtual double step () {
            ++step_count;

            if (step_count != 0) {
                mutate();
            }

            for (size_t i = 0; i < this->entities.size(); ++i) {
                evaluate(i);
            }

            before_exec();

            // Sort the entity array after evaluating
            size_t count = 0;
            while (true) {
                try {
                    sort_entities();
                    break;
                } catch (std::runtime_error const &e) {
                    std::cout << "Caught: " << e.what() << std::endl;
                    ++count;
                    if (count > 10) {
                        std::cout << "could not recover" << std::endl;
                        exit(101);
                    }
                }
            }

            // Send a progress report to display
            double average = 0;
            for (auto entity : this->entities) {
                average += entity->get_value();
            }
            average = average / entities.size();

            double best = this->entities[0]->get_value();
            p_report report {
                    calling_thread_id,
                    (size_t)step_count,
                    best,
                    average,
                    best - std::get<0>(last_stats),
                    average - std::get<1>(last_stats)
            };
            display_add_data(report);

            std::get<0>(last_stats) = best;
            std::get<1>(last_stats) = average;

            return best;
        }


        int train (size_t rounds) {
            for (size_t i = 0; i < rounds; ++i) {
                step();

                if (flags::global->kill_thread_exec) {
                    return TRAIN_KILL;
                }
            }
            return TRAIN_DONE;
        }

        int train (size_t rounds, double goal, bool(*comparison)(double, double)) {
            for (size_t i = 0; i < rounds; ++i) {
                if (comparison(step(), goal)) {
                    return TRAIN_FIN;
                }

                if (flags::global->kill_thread_exec) {
                    return TRAIN_KILL;
                }
            }
            return TRAIN_DONE;
        }

    }; // class set_t<E>

}
