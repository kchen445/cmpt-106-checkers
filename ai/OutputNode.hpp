//
// Created by Jeremy S on 2017-09-26.
//
// network::OutputNode              (object)
//      A node type to represent an output node for the network.
//      When fired, this node sends a message using the internal
//      messaging system to send its data to an listening objects.

#ifndef NNETWORK_OUTPUTNODE_HPP
#define NNETWORK_OUTPUTNODE_HPP

#include "NodeType.hpp"
#include "../core/im/Sender.hpp"
#include <iostream>

namespace network {

    class OutputNode : public NodeType<double>,
                       public im::Sender
    {
    private:

        static size_t nextId;

        // A unique identifier for each OutputNode object.
        //
        // For the checkers network this will most likely
        // be a linear representation of a matrix index.
        // Ex.
        //      index(row: 1, col: 0) would be 4 on a 4 by 4, zero indexed matrix
        size_t id;

    public:

        OutputNode()
                : NodeType<double>(),
                  im::Sender(im::Channel::neuralOutputNode),
                  id(OutputNode::nextId++)
        {}

        // Bypass activation function
        double activate () {
            return rawValue;
        }

        // Send a message to channel neuralOutputNode
        // with the identifier for this node and the
        // value that the node has.
        void send () {
            sendMessage({{id, rawValue}});
        }

    };

}

size_t network::OutputNode::nextId = 0;


#endif //NNETWORK_OUTPUTNODE_HPP
