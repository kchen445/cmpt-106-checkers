//
// Created by Jeremy S on 2017-11-19.
//

#pragma once

#include "set_t.hpp"

namespace ml {

    // Prototype for the routine to be called on thread start.
    template<typename E>
    void *t_exec (void *arg);

    template<typename E>
    class l_thread {
    public:

        // The thread object.
        pthread_t thread;

        // The current state of this thread.
        bool active = false;

        // The number of training rounds to be done.
        size_t rounds;

        // A pointer to the learning set to use for training.
        ptr<set_t<E>> l_set;

        // The id of this thread.
        size_t id;

        l_thread (ptr<set_t<E>> l_set, size_t id, size_t rounds)
                : l_set(),
                  id(id),
                  rounds(rounds)
        {
            this->l_set = l_set;
            l_set->calling_thread_id = this->id;
        }


        void start () {
            int rc = pthread_create(&thread, NULL, t_exec<E>, (void *)this);
            if (rc) {
                throw std::runtime_error("ml::l_thread: unable to create thread");
            }
        }

    };

    // The routine to be called once the thread starts.
    // arg should be a pointer to a l_thread<E> object.
    template<typename E>
    inline void *t_exec (void *arg) {
        auto ptr = (l_thread<E> *)arg;
        ptr->active = true;

        if (cfg::global->use_goal) {
            if (ptr->l_set->train(ptr->rounds, cfg::global->goal, cfg::global->fit_val_comp) == TRAIN_FIN) {
                flags::global->kill_thread_exec = true;
            }
        } else {
            ptr->l_set->train(ptr->rounds);
        }

        ptr->active = false;
        pthread_exit(nullptr);
    }

}
