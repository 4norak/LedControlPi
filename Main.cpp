#include "LedControl.h"

int main(void)
{
  LedControl display(10, 11, 8, 1);
  
  display.setLed(0, 0, 0, 1);
  delay(1000);
  display.setRow(0, 0, 0b11111111);
  delay(1000);
  display.setColumn(0, 0, 0b11111111);
  delay(1000);

  return 0;
}
