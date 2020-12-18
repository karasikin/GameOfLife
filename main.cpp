#include <iostream>

#include "world.h"
#include <unistd.h>

int main() {

    World w{ {
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 1, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 1, 0, 0, 0, 0, 0, 0,
            0, 1, 1, 1, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

            0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
             }, {10, 10} };


    while(true) {
        for(auto i{ 0ul }; i < w.getWorld()->size(); ++i) {
            std::cout << (w.getWorld()->at(i) ? '1' : ' ');
            if((i + 1) % w.x() == 0) {
                std::cout << std::endl;
            }
        }

        w.step();
        sleep(1);
        system("cls");
    }


    return 0;
}
