#include <iostream>
#include "ThresholdNode.hpp"
#include "InputNode.hpp"
#include "OutputNode.hpp"

using network::ThresholdNode;
using network::OutputNode;
using network::InputNode;

int main () {
    // -- Set Up -- //
    InputNode inputNode_1{1};
    InputNode inputNode_2{1};
    InputNode inputNode_3{0};
    InputNode inputNode_4{0};
    InputNode inputNode_5{1};

    OutputNode outputNode_1{1};
    OutputNode outputNode_2{1};
    OutputNode outputNode_3{2};
    OutputNode outputNode_4{1};

    ThresholdNode internalNode_1{4};
    ThresholdNode internalNode_2{3};
    ThresholdNode internalNode_3{2};

    inputNode_1.addConnection(&internalNode_2, -0.25);

    inputNode_2.addConnection(&internalNode_2, 0.5);
    inputNode_2.addConnection(&internalNode_1, 0.10);

    inputNode_3.addConnection(&internalNode_1, 0.25);

    inputNode_4.addConnection(&internalNode_1, 1);

    inputNode_5.addConnection(&internalNode_1, -1);

    internalNode_1.addConnection(&internalNode_2, 0.75);
    internalNode_1.addConnection(&internalNode_3, 0.5);
    internalNode_1.addConnection(&outputNode_3, 0.7);
    internalNode_1.addConnection(&outputNode_4, 0.23);

    internalNode_2.addConnection(&outputNode_1, -0.1);
    internalNode_2.addConnection(&outputNode_2, 0.9);
    internalNode_2.addConnection(&internalNode_3, 0.23);

    internalNode_3.addConnection(&outputNode_3, -0.9);


    // -- Running -- //

    inputNode_1.send();
    inputNode_2.send();
    inputNode_3.send();
    inputNode_4.send();
    inputNode_5.send();

}