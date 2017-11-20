//
// Created by Jeremy S on 2017-11-19.
//

#pragma once

#include "config.hpp"

namespace ml {

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

    using network_o = network_t<NEURAL_IN, NEURAL_OUT>;

}
