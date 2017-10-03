#ifndef NNETWORK_CONNECTIONTYPE_HPP
#define NNETWORK_CONNECTIONTYPE_HPP

namespace network {

    template<typename T, typename Node>
    struct Connection {

        Node* node;

        T weight;

        Connection (Node* fromNode, T const &weight) 
                : node(fromNode), weight(weight)
        {}

        Connection ()
                : node(nullptr), weight(0)
        {}

        Connection (Connection<T, Node> const &other)
                : node(other.node), weight(other.weight)
        {}

        T getValue () {
            return weight * node->calculate();
        }

    };

}

#endif
