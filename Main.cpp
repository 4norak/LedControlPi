#include "LedControl.h"
#include <signal.h>
#include<iostream>

bool shutdown = false;
LedControl display(10, 11, 8, 8);

void handleSig(int parameter) {
    std::cout << "Signal caught" << std::endl;
    shutdown = true;
    display.clearUp();
}

int main(void)
{
    signal(SIGINT, handleSig);
    signal(SIGTERM, handleSig);
    while(!shutdown)
        for(char i=48;i<58;i++) {
            display.setCharacter(0, i);
            delay(980);
            display.clearDisplay(0);
            delay(20);
        }

    return 0;
}
