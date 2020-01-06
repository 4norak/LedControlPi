#include "LedControl.h"

int main(void)
{
  LedControl display(10, 11, 8, 1);
  display.shutdown(0, false);
  display.setIntensity(0, 8);
  display.clearDisplay(0);
  
  display.setLed(0, 0, 0, 1);

  delay(5000);

  return 0;
}
