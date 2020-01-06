#include "LedControl.h"

int main(void)
{
  LedControl display(10, 11, 8, 1);
  lc.shutdown(0, false);
  lc.setIntensity(0, 8);
  lc.clearDisplay(0);
  
  display.setLed(0, 0, 0, 1);

  delay(5000);

  return 0;
}
