#pragma once

#include <memory>
#include "NetworkType.hpp"

namespace ml {

    template<size_t In, size_t Out>
    class LearningEntity {
    public:

        std::shared_ptr<NetworkType<In, Out>> network;


        LearningEntity (std::shared_ptr<NetworkType<In, Out>> network)
            : network(network)
        {}

        virtual ~LearningEntity () {}


        // Should return the fitness value for this entity.
        virtual double getValue () const = 0;

        // Resets any internal values uesd to compute getValue();
        virtual void reset () {}

        // Operator < which will be used to sort entities.
        // The default implimentation compares the restult of the getValue()
        // method for each object using <. This will result in the best entity
        // being the one with the lowest value. If this is not desired for
        // subclasses, they may use > when comparing values so that the best
        // value is the one with the highest value.
        virtual bool operator< (LearningEntity<In, Out> const &other) {
            return getValue() < other.getValue();
        }

    };

}
