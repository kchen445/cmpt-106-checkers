#include <iostream>
#include "MLFramework.hpp"
#include "CompetitiveLearning.hpp"
#include "TargetedLearning.hpp"

namespace test {
    void a () {
        std::cout << __PRETTY_FUNCTION__ << ": hello" << std::endl;
    }
}

int main () {
    test::a();
    std::cout << "OK!" << std::endl;
}
