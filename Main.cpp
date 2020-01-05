#include "LedControl.h"

int main(void)
{
  LedControl display(10, 11, 7);

  for(int i = 0; i < 8; i++)
    display.setLed(i, i, i, 1);

  delay(5000);

  return 0;
}
