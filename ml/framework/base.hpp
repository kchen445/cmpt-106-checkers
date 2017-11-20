//
// Created by Jeremy S on 2017-11-19.
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

// As these are used a lot I'll define shorter names for these.
template<typename T>
using ptr = std::shared_ptr<T>;

template<typename T>
using uptr = std::unique_ptr<T>;

// Utility functions
namespace util {


    inline double abs (double a) {
        return a < 0 ? -a : a;
    }

    inline int abs (int a) {
        return a < 0 ? -a : a;
    }


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
