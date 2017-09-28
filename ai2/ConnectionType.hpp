//
// Created by Jeremy S on 2017-09-26.
//

#ifndef NNETWORK_CONNECTIONTYPE_HPP
#define NNETWORK_CONNECTIONTYPE_HPP

namespace network {

    template<typename T, typename Node>
    struct ConnectionType {

        Node* node;
        T weight;

        ConnectionType (Node* node, double weight)
                : weight(weight), node(node)
        {}

        void feedForward (T const &value) {
            node->rawValue += (weight * value);
            /*node->numInputs--;
            if (node->numInputs == 0) {
                node->send();
            }*/
        }

    };

}



#endif //NNETWORK_CONNECTIONTYPE_HPP
