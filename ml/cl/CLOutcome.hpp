#pragma once

#include <exception>
#include <memory>
#include <tuple>
#include "CLEntity.hpp"

namespace ml {

    enum class CLOutcome {
        win, loss, tie
    };

    // Using declaration for the outcome of a 2 player game.
    using CLDualOutcome = std::tuple<CLOutcome, CLOutcome>;


    // Converts an integer to CLOutcome type.
    inline CLOutcome toCLOutcome (int x) {
        switch (x) {
        case    0: return CLOutcome::tie;
        case    1: return CLOutcome::win;
        case   -1: return CLOutcome::loss;
        default: throw std::runtime_error("ml::toCLOutcome(int): Unhandled input");
        }
    }

    // Applies an outcome to a CLEntity, incrementing the correct variable in the object.
    template<size_t In, size_t Out>
    inline void apply (CLOutcome outcome, std::shared_ptr<CLEntity<In, Out>> entity) {
        switch (outcome) {
        case CLOutcome::win:
            entity->wins++;
            break;
        case CLOutcome::loss:
            entity->losses++;
            break;
        case CLOutcome::tie:
            entity->ties++;
            break;
        }
    }

    // Applies a dual outcome to two entities.
    template<size_t In, size_t Out>
    inline void apply (CLDualOutcome outcome,
                       std::shared_ptr<CLEntity<In, Out>> e1,
                       std::shared_ptr<CLEntity<In, Out>> e2)
    {
        apply(std::get<0>(outcome), e1);
        apply(std::get<1>(outcome), e2);
    }

}
