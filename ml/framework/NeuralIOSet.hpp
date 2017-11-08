#pragma once

#include <array>
#include <vector>
#include <cmath>
#include <exception>
#include "util.hpp"
#include "Config.hpp"
#include "NetworkType.hpp"

namespace ml {

    template<size_t N>
    using InputSet = std::array<double, N>;

    template<size_t N>
    using OutputSet = std::array<double, N>;


    // A data structure to represent the input and output of a network.
    // Used in targeted learning to determine how well the network is
    // preforming.
    template<size_t In, size_t Out>
    struct NeuralIOPair {

        InputSet<In> in;
        OutputSet<Out> out;

    };


    // Returns the difference between two neural io pair objects.
    template<size_t In, size_t Out>
    inline double difference (NeuralIOPair<In, Out> const &base, NeuralIOPair<In, Out> const &ref) {
        std::vector<double> diffs;
        for (size_t i = 0; i < Out; ++i) {
            diffs.push_back(abs(ref[i] - base[i]));
        }

        size_t maxIdx = util::max_index(diffs);
        double sum = 0.0;
        for (size_t i = 0; i < diffs.size(); ++i) {
            if (i != maxIdx) {
                sum += diffs[i] * Config::global->differenceCoef;
            } else {
                sum += diffs[i];
            }
        }
        return sum;
    }


    // A data structure to represent a set of neural io pair objects.
    // Used in targeted learning as a reference set to train a network.
    //  e.g.
    //  The set for training a network to solve the xor of two inputs is:
    //
    //     {in: [0,0] -> out: [0],
    //      in: [0,1] -> out: [1],
    //      in: [1,0] -> out: [1],
    //      in: [1,1] -> out: [0]}
    //      (note: this is not proper syntax)
    template<size_t In, size_t Out>
    struct NeuralIOSet {

        std::vector<NeuralIOPair<In, Out>> definitions;

        // Construct this object for a set of reference pairs.
        // Use this constructor when defining a learning target.
        NeuralIOSet (std::vector<NeuralIOPair<In, Out>> const &referencePairs)
            : definitions(referencePairs)
        {}

        // Construct this object by asking a network to solve a set of inputs.
        // Use this constructor when training networks.
        NeuralIOSet (NetworkType<In, Out>& network, std::vector<std::array<double, In>> const &inputs)
            : definitions()
        {
            // Append the solution of each network to definitions.
            for (size_t i = 0; i < inputs.size(); ++i) {
                const std::array<double, In>& input = inputs[i];
                definitions.push_back({input, network.solve(input)});
            }
        }


        // Returns all the inputs for this set.
        std::vector<InputSet<In>> inputs () const {
            std::vector<InputSet<In>> v;
            for (size_t i = 0; i < definitions.size(); ++i) {
                v.push_back(definitions[i].in);
            }
            return v;
        }

        // Returns all the outputs for this set.
        std::vector<InputSet<In>> outputs () const {
            std::vector<OutputSet<Out>> v;
            for (size_t i = 0; i < definitions.size(); ++i) {
                v.push_back(definitions[i].out);
            }
            return v;
        }


        // Returns the difference between two sets.
        double difference (NeuralIOSet<In, Out> const &reference) {
            std::vector<double> diffs;
            
            if (definitions.size() != reference.definitions.size()) {
                throw std::runtime_error(__PRETTY_FUNCTION__ + ": different number of definitions in each set.");
            }

            for (size_t i = 0; i < definitions.size(); ++i) {
                diffs.push_back(difference(definitions[i], reference.definitions[i]));
            }

            double sum = 0.0;
            size_t maxIdx = util::max_index(diffs);
            for (size_t i = 0; i < diffs.size(); ++i) {
                if (i == maxIdx) {
                    sum += diffs[i];
                } else {
                    sum += Config::global->differenceCoef * diffs[i];
                }
            }

            return sum;
        }

    };

}
