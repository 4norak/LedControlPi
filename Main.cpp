#include "LedControl.h"
#include <signal.h>
#include<iostream>

bool shutdown = false;
LedControl display(10, 11, 8, 8);

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
            for(int j=0; j<8; j++)
                display.setCharacter(j, i);
            delay(980);
            for(int j=0; j<8; j++)
                display.clearDisplay(j);
            delay(20);
        }

    return 0;
}
