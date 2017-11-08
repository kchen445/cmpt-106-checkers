#pragma once

#include <memory>

namespace ml {

    class Flags {
    public:

        static std::shared_ptr<Flags> global;

        bool killThreadExecution = false;

    };

}

std::shared_ptr<ml::Flags> ml::Flags::global = std::shared_ptr<ml::Flags>{new ml::Flags{}};
