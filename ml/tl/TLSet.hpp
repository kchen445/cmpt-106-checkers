#pragma once

#include <memory>
#include "../MLFramework.hpp"
#include "TLEntity.hpp"

namespace ml {

    template<size_t In, size_t Out>
    class TLSet : public LearningSet<TLEntity<In, Out>, NUM_ENTITIES_PER_SET> {
    public:

        std::shared_ptr<NeuralIOSet<In, Out>> target;

        TLSet (std::shared_ptr<NeuralIOSet<In, Out>> target)
            : LearningSet<TLEntity<In, Out>, NUM_ENTITIES_PER_SET>(),
              target(target)
        {
            // TODO: populate the entities array with TLEntity objects
        }

        TLSet (std::shared_ptr<NeuralIOSet<In, Out>> target, 
               std::shared_ptr<NetworkType<In, Out>> seed)
            : LearningSet<TLEntity<In, Out>, NUM_ENTITIES_PER_SET>(seed),
              target(target)
        {}

        void evaulate (size_t i) override {
            // Cast the pointer at entities[i] to TLEntity type.
            // TLEntity<In, Out> *entity = 
            //         dynamic_cast<TLEntity<In, Out>>(this->entities[i].get());
            // Set that enetity's difference value to be the difference 
            // between the target and solution NueralIOSet.
            this->entities[i]->differenceValue = 
                    NeuralIOSet<In, Out>{this->entities[i]->network, target->inputs()}.differemce(target);
        }

    };

}
