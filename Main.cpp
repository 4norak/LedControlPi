#include "LedControl.h"

int main(void)
{
  LedControl display(10, 11, 8, 8);
  
  display.setCharacter(0, '3');
  display.setCharacter(1, '6');
  display.setCharacter(2, '7');
  display.setCharacter(3, '9');
  display.setCharacter(4, '3');
  display.setCharacter(5, '6');
  display.setCharacter(6, '7');
  display.setCharacter(7, '9');

  delay(200);

  return 0;
}
