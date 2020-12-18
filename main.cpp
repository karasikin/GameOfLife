#include <iostream>

#include "world.h"


int main() {

    World w{ {
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

            0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
             }, {10, 10} };

    std::cout << std::endl << w.numberOfLivingAroundWithBorder(0) << std::endl;
    //std::cout << std::endl << w.numberOfLivingAroundWithBorder(0) << std::endl;

    //auto [x, y] = w.checkNeighbourhood(24);

    //std::cout << x << ' ' << y << std::endl;

    return 0;
}
