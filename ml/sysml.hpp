//
// Created by Jeremy S on 2017-11-10.
//

#pragma once

#include "framework/base.hpp"
#include "framework/clock.hpp"
#include "framework/config.hpp"
#include "framework/display.hpp"
#include "framework/neural_io.hpp"
#include "framework/network_t.hpp"
#include "framework/entity_t.hpp"
#include "framework/set_t.hpp"
#include "framework/l_thread.hpp"
#include "framework/core_t.hpp"


// Legacy declarations
namespace ml {
    template<size_t In, size_t Out>
    using NetworkType = ml::network_t<In, Out>;
}
namespace network {
    template<size_t In, size_t Out>
    using NetworkType = ml::network_t<In, Out>;
}

