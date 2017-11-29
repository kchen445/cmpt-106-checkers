//
// Created by Jeremy S on 2017-11-10.
//

#pragma once

#include <cstdlib>
#include <cmath>
#include "sysml.hpp"

struct neuron {


    struct conn {
        ptr<neuron> target;
        double weight;

        conn (ptr<neuron> const &target, double weight)
                : target(), weight(weight)
        {
            this->target = target;
        }

        explicit conn (ptr<neuron> const &target)
                : target(), weight(util::random_range(-1, 1))
        {
            this->target = target;
        }

        double feed_forward () {
            return this->weight * this->target->get_value();
        }
    };


    double value;
    std::vector<ptr<conn>> conns;
    double bias = 0;

    neuron ()
            : value(0), conns()
    {}

    virtual double act_f (double in) {
        return tanh(in * 0.5);
    }

    virtual double get_value () {
        this->value = bias;
        for (auto const &conn : conns) {
            this->value += conn->feed_forward();
        }
        this->value = act_f(value);
        return this->value;
    }
};


using conn = neuron::conn;



struct in_neuron : public neuron {

    in_neuron ()
            : neuron()
    {}

    double get_value () override {
        return this->value;
    }

    void set_value (double x) {
        this->value = x;
    }

};


// connects the base neuron to the target one and returns a pointer to the connection object
inline ptr<conn> connect (ptr<neuron> const &base, ptr<neuron> const &target) {
    ptr<conn> c = ptr<conn>{new neuron::conn{target}};
    base->conns.push_back(c);
    return c;
}


template<size_t In, size_t Out>
struct net : public ml::NetworkType<In, Out> {

    std::array<ptr<in_neuron>, In> inset;
    std::array<ptr<neuron>, Out> outset;
    std::vector<ptr<neuron>> hset;

    std::vector<ptr<neuron>> nset;
    std::vector<ptr<conn>> cset;


    explicit net (size_t numh)
            : inset(), outset(), hset(), nset(), cset()
    {
        // create nodes
        for (size_t i = 0; i < In; ++i) {
            inset[i] = ptr<in_neuron>{new in_neuron()};
            nset.push_back(ptr<neuron>{inset[i]});
        }
        for (size_t i = 0; i < numh; ++i) {
            hset.push_back(ptr<neuron>{new neuron()});
            nset.push_back(ptr<neuron>{hset[i]});
        }
        for (size_t i = 0; i < Out; ++i) {
            outset[i] = ptr<neuron>{new neuron()};
            nset.push_back(ptr<neuron>{outset[i]});
        }
        // create connections
        for (ptr<neuron> const &base : outset) {
            for (ptr<neuron> const &target : hset) {
                cset.push_back(connect(base, target));
            }
        }
        for (ptr<neuron> const &base : hset) {
            for (ptr<neuron> const &target : inset) {
                cset.push_back(connect(base, target));
            }
        }
    }


    std::array<double, Out> solve (std::array<double, In> const &input) override {
        for (size_t i = 0; i < In; ++i) {
            inset[i]->set_value(input[i]);
        }
        std::array<double, Out> output{};
        for (size_t i = 0; i < Out; ++i) {
            output[i] = outset[i]->get_value();
        }
        return output;
    }


    void save (std::string const &filename) override {};


    ptr<ml::NetworkType<In, Out>> clone () override {
        ptr<net<In, Out>> other = ptr<net<In, Out>>{new net<In, Out>{hset.size()}};
        for (size_t i = In; i < nset.size(); ++i) {
            other->nset[i]->bias = this->nset[i]->bias;
        }
        for (size_t i = 0; i < cset.size(); ++i) {
            other->cset[i]->weight = this->cset[i]->weight;
        }
        return other;
    }


    void tweakWeight (int chance, double range) override {
        for (const ptr<conn> &c : this->cset) {
            if (util::random_chance(chance)) {
                c->weight = util::random_crange(c->weight, range);
            }
        }
    }

    void randomizeWeight (int chance) override {
        for (const ptr<conn> &c : this->cset) {
            if (util::random_chance(chance)) {
                c->weight = util::random_range(-1, 1);
            }
        }
    }

    void tweakBias (int chance, double range) override {
        for (const ptr<neuron> &n : this->nset) {
            if (util::random_chance(chance)) {
                n->bias = util::random_crange(n->bias, range);
            }
        }
    }

    void randomizeBias (int chance) override {
        for (const ptr<neuron> &n : this->nset) {
            if (util::random_chance(chance)) {
                n->bias = util::random_range(-1, 1);
            }
        }
    }

};
