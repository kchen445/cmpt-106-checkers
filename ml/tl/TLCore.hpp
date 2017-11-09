#pragma once

#include "TLEntity.hpp"
#include "../MLFramework.hpp"

namespace ml {

    template<size_t In, size_t Out>
    class TLCore : public LearningCore<TLEntity<In, Out>> {
    public:

        std::shared_ptr<NeuralIOSet<In, Out>> target;

        TLCore (std::shared_ptr<NeuralIOSet<In, Out>> target, 
                std::shared_ptr<NetworkType<In, Out>> seed)
            : LearningCore<TLEntity<In, Out>>(),
              target(target)
        {
            for (size_t i = 0; i < this->learningSets.size(); ++i) {
                this->learningSets[i] = TLSet<In, Out>{target, seed};
            }
        }

        // TODO: write constructor from config

    };

}
