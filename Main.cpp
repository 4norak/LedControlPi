#include "LedControl.h"

int main(void)
{
    LedControl display(10, 11, 8, 8);
    for(;;)
        for(int i=0;i<10;i++) {
            for(int j=0; j<8; j++)
                display.setCharacter(j, i);
            delay(980);
            for(int j=0; j<8; j++) {
                display.clearDisplay(j);
                delay(20);
            }
        }

    return 0;
}
