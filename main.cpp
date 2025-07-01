/**
 * This file is the controller for a simple c++ program which will allow a user to move a ball/square and 
 * push other objects out of the way
 */

#include "Display.h"

int main(){

    //init rand
    std::srand(static_cast<unsigned>(time(NULL)));

    Display simulation;

    while(simulation.isWinOpen()){
        simulation.update();
        simulation.render(); 
    }

    return 1;
}