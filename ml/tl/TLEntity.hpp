#pragma once

#include <memory>
#include "../MLFramework.hpp"

namespace ml {

    template<size_t In, size_t Out>
    class TLEntity : public LearningEntity<In, Out> {
    public:

        double differenceValue;

        TLEntity (std::shared_ptr<NetworkType<In, Out>> network)
            : LearningEntity<In, Out>(network),
              differenceValue(0.0)
        {}

        double getValue () const override {
            return differenceValue;
        }

    };

}
