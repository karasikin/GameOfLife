#include <iostream>
#include <thread>
#include <QApplication>

#include "world.h"
#include "worldview.h"



int main(int argc, char **argv) {

    QApplication app{ argc, argv};
    World world{ {4, 4} };
    WorldView world_view{ world };

    world_view.show();

    return app.exec();

//    World w{ {10, 10} };

//    w.set( {0, 1} );
//    w.set( {1, 2} );
//    w.set( {2, 0} );
//    w.set( {2, 1} );
//    w.set( {2, 2} );

//    w.set( {9, 1} );
//    w.set( {8, 2} );
//    w.set( {7, 0} );
//    w.set( {7, 1} );
//    w.set( {7, 2} );

//    w.setNeighborCountingPolicy(World::WITHOUT_BORDER);
//    w.setSavingLastStepChages(true);

//    do {
//        system("clear");
//        for(const auto &i : w.matrix()) {
//            for(const auto &j : i) {
//                std::cout << (j ? "x" : " ");
//            }
//            std::cout << std::endl;
//        }

//        for(auto &[x, y] : w.lastStapCellsChanged()) {
//            std::cout << "{" << x << "," << y << "} ";
//        }std::cout << std::endl;

//        std::this_thread::sleep_for(std::chrono::milliseconds(100));
//    } while(w.step());
}
