//
// Created by Jeremy S on 2017-11-10.
//

#pragma once

#include <cstdlib>
#include <cstdio>
#include <cmath>
#include <array>
#include <vector>
#include <tuple>
#include <memory>
#include <string>
#include <exception>
#include <iostream>
#include <cassert>
#include <pthread.h>
#include <chrono>
#include <thread>
#include <algorithm>

// Config that can not be defined as variables.
#define NUM_ENTITY_PER_SET  100

#ifndef NUM_THREADS
#define NUM_THREADS 4
#endif

#ifndef NEURAL_IN
#define NEURAL_IN   2
#endif

#ifndef NEURAL_OUT
#define NEURAL_OUT  1
#endif


// Training return values
#define TRAIN_DONE  0
#define TRAIN_KILL  9
#define TRAIN_FIN   1

#define KILLSYS     8


// As these are used a lot I'll define shorter names for these.
template<typename T>
using ptr = std::shared_ptr<T>;

template<typename T>
using uptr = std::unique_ptr<T>;



// Utility functions
namespace util {


    namespace func {

        // Generic prefix style operator<
        template<typename T>
        inline bool operator_lessthan (T lhs, T rhs) {
            return lhs < rhs;
        }

    }



    //                    //
    // Array Manipulation //
    //                    //


    // Returns the index of the largest element in v using a given comparator.
    // Comparison should should evaluate to true if rhs is larger than lhs.
    //  e.g. a less than operator.
    // Assumes v has at least one element.
    template<typename T>
    inline size_t max_index (std::vector<T> const &v, bool(*comparison)(T,T)) {
        size_t max = 0;
        for (size_t i = 0; i < v.size(); ++i) {
            if (comparison(v[max], v[i])) {
                max = i;
            }
        }
        return max;
    }

    // Overload of above with comparison being <
    template<typename T>
    inline size_t max_index (std::vector<T> const &v) {
        return max_index(v, func::operator_lessthan);
    }

    // Returns the largest element in a given vector via a given comparator.
    template<typename T>
    inline T max_elem (std::vector<T> const &v, bool(*comparison)(T,T)) {
        return v[max_index(v, comparison)];
    }

    template<typename T>
    inline T max_elem (std::vector<T> const &v) {
        return v[max_index(v)];
    }


    // Returns the index of the smallest element in v using a given comaprator.
    // Comparison should evaluate to true if lhs is less than rhs.
    //  e.g. a less than operator
    // Assumes v has at least one element.
    template<typename T>
    inline size_t min_index (std::vector<T> const &v, bool(*comparison)(T,T)) {
        size_t min = 0;
        for (size_t i = 0; i < v.size(); ++i) {
            if (comparison(v[i], v[min])) {
                min = i;
            }
        }
        return min;
    }

    template<typename T>
    inline size_t min_index (std::vector<T> const &v) {
        return min_index(v, func::operator_lessthan);
    }

    template<typename T>
    inline T min_elem (std::vector<T> const &v, bool(*comparison)(T,T)) {
        return v[min_index(v, comparison)];
    }

    template<typename T>
    inline T min_elem (std::vector<T> const &v) {
        return v[min_index(v)];
    }



    //               //
    // Randomization //
    //               //

    // Returns true a given percent of the time.
    // percent should be between 0 and 100.
    inline bool random_chance (int percent) {
        return (rand() % 100 < percent);
    }


    inline double random_range (double l, double u) {
        double r = double(rand()) / double(RAND_MAX);
        return (r * abs(u - l)) + l;
    }


    inline double random_crange (double c, double s) {
        return random_range(c - (0.5 * s), c + (0.5 * s));
    }

} // namespace util




namespace ml {


    // Config Singleton
    // This class holds all the settings for the system.
    // Values may be freely edited manualy or by the program
    // before or during training.
    //
    // As this is a singleton, access to the object must be
    // done through the static instance 'global'.
    struct cfg {
        static ptr<cfg> global;

        // difference settings
        double diff_coefficient = 0.1;

        // mutation settings
        bool mutate_best_network = false;
        size_t clone_num = 10;
        size_t tweak_num = 30;

        // display settings
        size_t output_interval = 1;

        // learning core settings
        bool use_goal = true;
        double goal = 0.015;
        std::chrono::milliseconds display_update_interval{250};
        size_t save_interval = 2;
        std::string save_path = "net.txt";
        bool pause_on_convergence = false;
        size_t convergence_interval = 500;

        // cl settings
        bool ties_count_as_wins = false;

        // comparator functions
        bool(*fit_val_comp)(double, double) = [](double e1, double e2) -> bool {
            return e1 < e2;
        };

    };

    ptr<cfg> cfg::global{new cfg{}};




    struct p_report {
        size_t thread_id;
        size_t round_num;
        double best;
        double average;
        double delta_best;
        double delta_average;
    };




    // Display Singleton
    class display {
    private:

        long next_index = 0;

        std::vector<std::vector<p_report>> data{};

        // Checks if each thread has given enough data to print out a report.
        bool is_ready_to_print () {
            for (size_t i = 0; i < data.size(); ++i) {
                if (((long)data[i].size() - 1) < next_index) {
                    return false;
                }
            }
            return true;
        }

        std::string format_f (const char *format, double val) {
            char str[32];
            std::string prefix = val < 0 ? "-" : " ";
            sprintf(str, format, abs(val));
            return prefix + std::string{str};
        }

        std::string format_i (const char *format, int val) {
            auto str = new char[32];
            sprintf(str, format, abs(val));
            return std::string{str};
        }

    public:
        static ptr<display> interface;

        // Setup the rows for the data matrix.
        void setup () {
            for (size_t i = 0; i < NUM_THREADS; ++i) {
                data.emplace_back(std::vector<p_report>{});
            }
        }

        // Adds a report to the specific row based on the thread id enclosed in the report.
        void add (p_report const &report) {
            data.at(report.thread_id).push_back(report);
        }

        void print_report (bool force = false) {
            if (!is_ready_to_print() && !force) {
                return;
            }

            std::string indent = "          ";
            std::string separator{2, ' '};

            const char *numericalFormat = "%.5f";
            char EOL = '\n';

            std::string title;
            if (!force) {
                title = format_i("Report %d", (int)next_index);
            } else {
                title = "Report (forced)";
            }

            std::string header = indent;
            std::string underline = indent;
            for (size_t i = 0; i < data.size(); ++i) {
                header += format_i("Thread %d", (int)i) + separator;
                underline += "--------" + separator;
            }

            std::string rowBest     = "Best    : ";
            std::string rowAverage  = "Average : ";
            std::string rowDeltaB   = "Delta B : ";
            std::string rowDeltaA   = "Delta A : ";

            for (auto row : data) {
                if (!force) {
                    rowBest += format_f(numericalFormat, row[next_index].best) + separator;
                    rowAverage += format_f(numericalFormat, row[next_index].average) + separator;
                    rowDeltaB += format_f(numericalFormat, row[next_index].delta_best) + separator;
                    rowDeltaA += format_f(numericalFormat, row[next_index].delta_average) + separator;
                } else {
                    if (!row.empty()) {
                        rowBest += format_f(numericalFormat, row.back().best) + separator;
                        rowAverage += format_f(numericalFormat, row.back().average) + separator;
                        rowDeltaB += format_f(numericalFormat, row.back().delta_best) + separator;
                        rowDeltaA += format_f(numericalFormat, row.back().delta_average) + separator;
                    } else {
                        rowBest += "********" + separator;
                        rowAverage += "********" + separator;
                        rowDeltaB += "********" + separator;
                        rowDeltaA += "********" + separator;
                    }
                }
            }

            std::string masterString =
                    title + EOL
                    + header + EOL
                    + underline + EOL
                    + rowBest + EOL
                    + rowAverage + EOL
                    + rowDeltaB + EOL
                    + rowDeltaA + EOL;

            next_index += cfg::global->output_interval;

            std::cout << masterString << std::endl;
        }

    }; // class display

    ptr<display> display::interface{new display{}};




    // Flag Singleton
    struct flags {
        static ptr<flags> global;

        bool kill_thread_exec = false;
    };

    ptr<flags> flags::global{new flags{}};




    // Abstract network type class
    template<size_t In, size_t Out>
    class network_t {
    public:
        virtual std::array<double, Out> solve (std::array<double, In> const &input) = 0;

        virtual void tweakWeight (int chance, double range) = 0;
        virtual void randomizeWeight (int chance) = 0;

        virtual void tweakBias (int chance, double range) = 0;
        virtual void randomizeBias (int chance) = 0;

        // Should return a network with the same configuration.
        // The returned network should be an isolated entity that
        // shares no pointers or anything with the base network.
        //
        // The network should be declared used the new operator.
        virtual ptr<network_t<In, Out>> clone () = 0;

        virtual void save (std::string const &filename) = 0;

        virtual ~network_t () = default;

    }; // class network_t



    // A data structure to represent the input and output for a network.
    template<size_t In, size_t Out>
    struct io_pair {
        std::array<double, In> input;
        std::array<double, Out> output;
    };


    // Returns a single floating point number that represents the difference
    // between two io_pair objects. Only the output variable is used to when
    // calculating the value so comparing objects with different inputs is
    // valid but will not give a meaning full result.
    //
    // The difference value is calculated using a weighted sum of all the
    // differences between each element in the output arrays of the objecst.
    // The closer the value is to 0 the close each object is to one another.
    template<size_t In, size_t Out>
    inline double difference (io_pair<In, Out> const &base, io_pair<In, Out> const &ref) {
        std::vector<double> diffs{};
        for (size_t i = 0; i < Out; ++i) {
            diffs.push_back(abs(ref.output[i] - base.output[i]));
        }

        size_t max_idx = util::max_index(diffs);
        double sum = 0;
        for (size_t i = 0; i < diffs.size(); ++i) {
            if (i == max_idx) {
                sum += diffs[i];
            } else {
                sum += diffs[i] * cfg::global->diff_coefficient;
            }
        }

        return sum;
    } // inline double difference ()


    // A data structure for representing a set of input output pairs.
    // Like io_pair, this class also has a difference function which uses a
    // similar weighted sum to calculate how similar a set is to a referece.
    template<size_t In, size_t Out>
    struct io_set_t {
        std::vector<io_pair<In, Out>> definitions;

        // Construct this object from a set of predefined values.
        explicit io_set_t (std::vector<io_pair<In, Out>> const &reference_pairs)
                : definitions(reference_pairs)
        {}

        io_set_t (std::vector<std::array<double, In>> const &in,
                std::vector<std::array<double, Out>> const &out)
                : definitions()
        {
            assert(in.size() == out.size());
            for (size_t i = 0; i < in.size(); ++i) {
                definitions.emplace_back(io_pair<In, Out>{in[i], out[i]});
            }
        }

        // Construct this object by asking a network to solve a set of inputs.
        io_set_t (ptr<network_t<In, Out>> const &network, std::vector<std::array<double, In>> const &inputs)
                : definitions()
        {
            for (size_t i = 0; i < inputs.size(); ++i) {
                definitions.emplace_back(io_pair<In, Out>{inputs[i], network->solve(inputs[i])});
            }
        }

        // Returns only the inputs from the set of pairs.
        std::vector<std::array<double, In>> inputs () const {
            std::vector<std::array<double, In>> ins{};
            for (auto const &p : definitions) {
                ins.push_back(p.input);
            }
            return ins;
        }

        // Returns only the outputs from the set of pairs.
        std::vector<std::array<double, Out>> outputs () const {
            std::vector<std::array<double, Out>> outs{};
            for (auto const &p : definitions) {
                outs.push_back(p.output);
            }
            return outs;
        }

        // Returns a scalar value which denotes the difference between this
        // set and a reference set. Uses the same weighted sum to determin
        // the value as the difference function for io_pair.
        double difference (io_set_t<In, Out> const &ref) {
            if (definitions.size() != ref.definitions.size())
                throw std::runtime_error("ml::io_set: miss matching set sizes");

            std::vector<double> diffs;
            for (size_t i = 0; i < definitions.size(); ++i) {
                diffs.push_back(ml::difference(definitions[i], ref.definitions[i]));
            }
            double sum = 0;
            size_t max_idx = util::max_index(diffs);
            for (size_t i = 0; i < diffs.size(); ++i) {
                if (i == max_idx) {
                    sum += diffs[i];
                } else {
                    sum += diffs[i] * cfg::global->diff_coefficient;
                }
            }
            return sum;
        }

    }; // class io_set

    using io_set = io_set_t<NEURAL_IN, NEURAL_OUT>;


    // An non-template definition of network_t.
    using network_o = network_t<NEURAL_IN, NEURAL_OUT>;

    // An abstract class to represent a learning entity.
    struct entity_t {

        ptr<network_o> network;

        explicit entity_t (ptr<network_o> n)
                : network(std::move(n))
        {}

        explicit entity_t (network_o *n)
                : network(n)
        {}

        virtual ~entity_t () = default;

        // Returns the fitness value for this entity.
        virtual double get_value () const = 0;

        // Reset any internal variables used to calculate get_value()
        virtual void reset () {};

        // Operator < which will be used to sort entities.
        // The default implimentation compares the restult of the get_value()
        // method for each object using <. This will result in the best entity
        // being the one with the lowest value. If this is not desired for
        // subclasses, they may use > when comparing values so that the best
        // value is the one with the highest value.
        virtual bool operator< (entity_t const &other) {
            return get_value() < other.get_value();
        }

    }; // struct entity_t




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
            populate(seed);
        }

        virtual ~set_t () = default;

        // Populates this set with entities based off of a given seed.
        // This set should be empty before calling this method.
        //
        // Alternativly the seed constructor may be used.
        void populate (ptr<network_o> seed) {
            for (auto &entity : entities) {
                auto seed_variant = seed->clone();
                seed_variant->tweakWeight(10, 0.1);
                entity = ptr<E>{new E{seed_variant}};
            }
        }

        // Evaluates the entity at a given index.
        // The results of this evaluation should be stored in the entity.
        virtual void evaluate (ptr<E> const &e) = 0;

        // Called before evaluating the entities.
        virtual void before_exec () {}

        // Sort the entities usiing the operator < overload in the entity_t class.
        virtual void sort_entities () {
            std::sort(entities.begin(), entities.end(), [](ptr<E> const &e1, ptr<E> const &e2) -> bool {
                return e1->get_value() < e2->get_value();
            });
        }

        // Loops through all entities mutating them based on their position in the array.
        // The entities array should be sorted before this method is called.
        void mutate () {
            size_t start_id = config->mutate_best_network ? 0 : 1;
            for (size_t i = start_id; i < entities.size(); ++i) {
                if (i > (entities.size() - 1 - config->clone_num)) {
                    entities[i]->network = entities[0]->network->clone();
                    entities[i]->network->tweakWeight(20, 0.1);
                    entities[i]->network->tweakBias(10, 0.1);
                } else if (i < config->tweak_num) {
                    entities[i]->network->tweakWeight(60, 0.3);
                    entities[i]->network->tweakBias(40, 0.2);
                } else {
                    entities[i]->network->randomizeWeight(100);
                    entities[i]->network->randomizeBias(100);
                }
            }
        }


        double step () {
            ++step_count;

            if (step_count != 0) {
                mutate();
            }

            for (auto const &e : entities) {
                evaluate(e);
            }

            // Sort the entity array after evaluating
            sort_entities();

            // Send a progress report to display
            double average = 0;
            for (auto entity : entities) {
                average += entity->get_value();
            }
            average = average / entities.size();

            double best = entities[0]->get_value();
            p_report report {
                    calling_thread_id,
                    (size_t)step_count,
                    best,
                    average,
                    best - std::get<0>(last_stats),
                    average - std::get<1>(last_stats)
            };
            display::interface->add(report);

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


    // Prototype for the routine to be called on thread start.
    template<typename E>
    void *t_exec (void *arg);

    template<typename E>
    class l_thread {
    private:

        // The thread object.
        pthread_t thread;

    public:
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



    // Class in charge of coordinating the threads
    template<typename E>
    class core_t {
    private:

        bool all_threads_finished () {
            bool done = true;
            for (auto t : threads) {
                done = done && !t->active;
            }
            return done;
        }

        int pause_interface (ptr<E> const &best_entity) {
            while (true) {

                try {

                    std::string command;
                    std::cout << "Command Interface [continue/quit/save]" << std::endl;
                    std::cout << " > ";
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
                        best_entity->network->save(cfg::global->save_path);
                    }

                    else if (command == "set") {
                        // TODO: implement this
                    }

                    else {
                        std::cout << "Invalid command" << std::endl;
                    }

                } catch (...) {
                    std::cout << "Unable to execute command." << std::endl;
                }
            }

            return 0;
        }

    protected:

        virtual void on_convergence (ptr<E> const &best_entity) {};

        virtual void sort_stats (std::array<std::tuple<int, double>, NUM_THREADS> &stats) {
            std::sort(stats.begin(), stats.end(), [](auto e1, auto e2) -> bool {
               return std::get<double>(e1) < std::get<double>(e2);
            });
        }

    public:

        std::array<ptr<set_t<E>>, NUM_THREADS> l_sets;

        std::array<ptr<l_thread<E>>, NUM_THREADS> threads;

        // Default contructor
        // *** NOTE ***
        // The default constructor does not populate the arrays just
        // initalizes them. This means that simply calling the default
        // constructor does not make this object ready for use.
        //
        // Arrays should be populated in the constructors of subclasses.
        core_t ()
                : l_sets(),
                  threads()
        {}

        virtual ~core_t () = default;


        // Runs a training routine.
        // To define and use a goal, do so in the Config singleton.
        // If no goal is defined training will run until terminated
        // by the user.
        ptr<E> train () {

            // Setup
            double best_val = 0;
            ptr<E> best_entity = nullptr;
            size_t convergence_count = 0;

            display::interface->setup();
            flags::global->kill_thread_exec = false;


            // Training loop
            do {

                // Start the threads
                for (auto t : threads) {
                    t->start();
                }

                // Wait till the threads have finished.
                // Periodically print out reports from display.
                while (!all_threads_finished()) {
                    // As to not be constantly querying display to print new data
                    // we'll put the main thread to sleep for a set amount of time.
                    std::this_thread::sleep_for(cfg::global->display_update_interval);
                    display::interface->print_report();
                }

                // -- all threads have stopped here -- //

                // Thread Convergence

                // Get the best entity from each thread.
                //  tuple form:
                //      (thread_id: int, fitness_value: double)
                std::array<std::tuple<int, double>, NUM_THREADS> t_stats;
                for (size_t i = 0; i < threads.size(); ++i) {
                    std::get<int>(t_stats[i]) = (int)i;
                    std::get<double>(t_stats[i]) = threads[i]->l_set->entities[0]->get_value();
                }
                sort_stats(t_stats);

                int best_t_id = std::get<int>(t_stats.front());
                int worst_t_id = std::get<int>(t_stats.back());

                // Set best entity to be the best entity from all the threads.
                best_entity = threads[best_t_id]->l_set->entities[0];
                best_val = best_entity->get_value();

                // Clone the best network and tweak it slightly.
                ptr<E> clone{new E{best_entity->network->clone()}};
                clone->network->tweakWeight(10, 0.1);

                // Insert the clone into the worst thread.
                threads[worst_t_id]->l_set->entities.back() = clone;

                // Do any extra stuff
                on_convergence(best_entity);

                // Auto save
                if (convergence_count % cfg::global->save_interval == 0) {\
                    std::cout << "Saving ..." << std::endl;
                    best_entity->network->save(cfg::global->save_path);
                }


                // Display convergence info and pause interface
                std::cout << " -- Convergence - " << convergence_count << " -- " << std::endl << std::endl;
                display::interface->print_report(true);

                // Print out the goal if there is one
                if (cfg::global->use_goal) {
                    std::cout << "Target: " << cfg::global->goal << std::endl;
                }
                std::cout << "Best  : " << best_entity->get_value() << std::endl << std::endl;

                // Pause interface
                if (cfg::global->pause_on_convergence && !flags::global->kill_thread_exec) {
                    std::cout << "Paused" << std::endl;
                    if (pause_interface(best_entity) == KILLSYS) {
                        return best_entity;
                    }
                }

                ++convergence_count;

            } while (!flags::global->kill_thread_exec);

            std::cout << "Training Complete!" << std::endl;

            std::cout << "Saving ..." << std::endl;
            best_entity->network->save(cfg::global->save_path);

            return best_entity;

        }

    }; // class core_t

} // namespace ml



// Legacy declarations
namespace ml {
    template<size_t In, size_t Out>
    using NetworkType = ml::network_t<In, Out>;
}
namespace network {
    template<size_t In, size_t Out>
    using NetworkType = ml::network_t<In, Out>;
}

