#include "LedControl.h"
#include <signal.h>
#include<iostream>

#define NUM_DISPLAYS 4

bool shutdown = false;
LedControl display(10, 11, 13, NUM_DISPLAYS);

void handleSig(int parameter) {
    std::cout << "Signal caught" << std::endl;
    shutdown = true;
    display.cleanUp();
}

int main(void)
{
    signal(SIGINT, handleSig);
    signal(SIGTERM, handleSig);
    while(!shutdown)
        for(char i=48;i<58;i++) {
            for(int j=0; j<NUM_DISPLAYS; j++)
                display.setCharacter(j, i);
            delay(1000);
        }

    return 0;
}
