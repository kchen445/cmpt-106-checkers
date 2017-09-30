//
// Created by Jeremy S, 9/29/17
//
// A list of all avaliable channels

// --- Update this file to add new channels when needed --- //

#ifndef IM_CHANNEL_HPP
#define IM_CHANNEL_HPP

namespace im {

    enum class Channel {
        testChan1,
        testChan2,


        // Message Contents:
        //  [0] (size_t): unique output node identifier
        //  [1] (double): output node value
        neuralOutputNode,

        // Message Contents:
        //  [0] (size_t): unique output node identifier
        neuralOutputNodeCreated

    };

}

#endif
