#include "LedControl.h"

int main(void)
{
  LedControl display(10, 11, 8, 8);
  
  for(int i=0; i<20; i++) {
    display.setCharacter(0, '3');
    display.setCharacter(1, '6');
    display.setCharacter(2, '7');
    display.setCharacter(3, '9');
    display.setCharacter(4, '3');
    display.setCharacter(5, '6');
    display.setCharacter(6, '7');
    display.setCharacter(7, '9');
    for(int j=0; j<8; j++)
        display.shutdown(i, true);
    for(int j=0; j<8; j++)
        display.shutdown(i, false);
    delay(20);
  }

  return 0;
}
