#include <iostream>
#include <ctime>

#define NEURAL_IN   3
#define NEURAL_OUT  3
#define NUM_THREADS 4

#include "sysml.hpp"
#include "network.hpp"
#include "systl.hpp"

template<typename T>
std::string as_str (std::vector<T> const &v) {
    std::string str = "[";
    for (size_t i = 0; i < v.size(); ++i) {
        str += std::to_string(v[i]);
        if (i != v.size() - 1) {
            str += ", ";
        }
    }
    str += "]";
    return str;
}

template<typename T, size_t N>
std::string as_str (std::array<T, N> const &v) {
    std::string str = "[";
    for (size_t i = 0; i < N; ++i) {
        str += std::to_string(v[i]);
        if (i != v.size() - 1) {
            str += ", ";
        }
    }
    str += "]";
    return str;
}


template<typename T, size_t N>
std::string as_str (std::vector<std::array<T, N>> const &v) {
    std::string str = "{";
    for (size_t i = 0; i < v.size(); ++i) {
        str += as_str(v[i]);
        if (i != v.size() - 1) {
            str += ", ";
        }
    }
    str += "}";
    return str;
}


int main() {
    srand((unsigned int)time(nullptr));


    ml::cfg::global->pause_on_convergence = false;
    ml::cfg::global->convergence_interval = 1000;
    ml::cfg::global->display_update_time = 500; // update time in milliseconds
    ml::cfg::global->use_goal = true;
    ml::cfg::global->goal = 0.025;


    auto n = ptr<ml::network_o>{new net<NEURAL_IN,NEURAL_OUT>{10}};

    std::vector<std::array<double, NEURAL_IN>> inputs {
            {0,0,0}, 
            {0,0,1}, 
            {0,1,0}, 
            {0,1,1},
            {1,0,0},
            {1,0,1},
            {1,1,0},
            {1,1,1}
    };

    std::vector<std::array<double, NEURAL_OUT>> outputs {
            {1,1,1}, 
            {1,1,0}, 
            {1,0,1}, 
            {1,0,0},
            {0,1,1},
            {0,1,0},
            {0,0,1},
            {0,0,0}
    };

    ptr<ml::io_set> tp{new ml::io_set{inputs, outputs}};

    tl::trainer trainer{n, tp};
    auto best = trainer.train();

    ml::io_set sol{best->network, inputs};
    std::cout << as_str(sol.outputs()) << std::endl;
}