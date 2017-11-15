//
// Created by Jeremy S on 2017-11-11.
//

#pragma once

#include "sysml.hpp"

namespace tl {

    struct entity : public ml::entity_t {

        double fit_val = 0;

        explicit entity (ptr<ml::network_o> const &n)
                : ml::entity_t(n)
        {}

        double get_value () const override {
            return fit_val;
        }
    };


    class set : public ml::set_t<entity> {
    public:

        ptr<ml::io_set> target;

        set (ptr<ml::network_o> const &seed, ptr<ml::io_set> target)
                : ml::set_t<entity>(seed),
                  target(std::move(target))
        {}

        void evaluate (size_t idx) override {
            ml::io_set io{entities[idx]->network, target->inputs()};
            this->entities[idx]->fit_val = io.difference(*target);
        }
    };


    class trainer : public ml::core_t<entity> {
    public:

        ptr<ml::io_set> target;

        trainer (ptr<ml::network_o> const &seed, ptr<ml::io_set> target)
                : ml::core_t<entity>(),
                  target(std::move(target))
        {
            for (size_t i = 0; i < NUM_THREADS; ++i) {
                auto seed_varient = seed->clone();
                if (i != 0) {
                    seed_varient->tweakWeight(10, 0.1);
                }
                this->l_sets[i] = ptr<set>{new tl::set{seed_varient, this->target}};
                this->threads[i] = ptr<ml::l_thread<entity>>{
                    new ml::l_thread<entity>(this->l_sets[i], i, ml::cfg::global->convergence_interval)
                };
            }
        }

    };

}
