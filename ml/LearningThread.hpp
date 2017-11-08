#pragma once 

#include <memory>
#include <pthread.h>
#include <exception>
#include "LearningSet.hpp"

namespace ml {


    namespace thread {

        // Prototype for function to be executed by the thread.
        template<size_t In, size_t Out, size_t N>
        void *threadExec (void *arg);

    }


    template<size_t In, size_t Out, size_t N>
    class LearningThread {
    private:

        pthread_t *thread;

    public:

        // Is the thread active or not?
        bool active = false;

        // The id of this thread.
        int id;

        // A pointer to the learning set used in this thread.
        std::shared_ptr<LearningSet<In, Out, N>> learningSet;

        // Should training be done using a goal?
        bool useGoal = false;

        // The goal used for training.
        double goal = 0.0;

        // The number of rounds of training to be done.
        size_t rounds;


        LearningThread (std::shared_ptr<LearningSet<In, Out, N>> learningSet, int id, size_t rounds)
            : rounds(rounds),
              learningSet(learningSet),
              thread(nullptr),
              id(id)
        {}

        ~LearningThread () {
            if (thread != nullptr) {
                delete thread;
            }
        }


        // Create the thread and begin execution.
        // Throws std::runtime_error if unable to create the thread.
        void start () {
            if (thread != nullptr) {
                delete thread;
                thread = nullptr;
            }

            int rc = pthread_create(thread, NULL, thread::threadExec<In, Out, N>, (void *)this);
            if (rc) {
                throw std::runtime_error("LearningThread::start(): unable to create thread.");
            }
        }


        // Tells the thread to train using a goal.
        // Calls start().
        void start (double goal) {
            this->useGoal = true; // set the goal flag, is reset once training is finished.
            this->goal = goal;
            start();
        }

    };


    namespace thread {

        // Function to be executed by the thread.
        // arg should be a pointer to the calling LearningThread object.
        template<size_t In, size_t Out, size_t N>
        inline void *threadExec (void *arg) {
            LearningThread<In, Out, N> *t_ptr = (LearningThread<In, Out, N> *)arg;
            t_ptr->active = true;

            if (t_ptr->useGoal) {
                t_ptr->learningSet->train(t_ptr->rounds, t_ptr->goal);
                t_ptr->useGoal = false; // reset the goal flag
            } else {
                t_ptr->learningSet->train(t_ptr->rounds);
            }

            t_ptr->active = false;
            pthread_exit(NULL);
        }

    }

}
