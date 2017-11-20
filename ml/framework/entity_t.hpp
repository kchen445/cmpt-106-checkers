//
// Created by Jeremy S on 2017-11-19.
//

#pragma once

#include "network_t.hpp"

namespace ml {

    // An abstract class to represent a learning entity.
    struct entity_t {

        ptr<network_o> network;

        entity_t () = default;

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

}
