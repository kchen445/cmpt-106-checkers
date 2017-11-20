//
// Created by Jeremy S on 2017-11-19.
//

#pragma once

#include "network_t.hpp"

namespace ml {

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
            diffs.push_back(util::abs(ref.output[i] - base.output[i]));
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
                definitions.push_back(io_pair<In, Out>{in[i], out[i]});
            }
        }

        // Construct this object by asking a network to solve a set of inputs.
        io_set_t (ptr<network_t<In, Out>> const &network, std::vector<std::array<double, In>> const &inputs)
                : definitions()
        {
            for (size_t i = 0; i < inputs.size(); ++i) {
                definitions.push_back(io_pair<In, Out>{inputs[i], network->solve(inputs[i])});
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

            std::vector<double> diffs{};
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

}
