#include <avr/io.h>

void enableLed(int lednumber);
void enableLeds(uint8_t ledbyte);
void enableAllLeds();
void lightUpLeds(uint8_t ledbyte);
void lightUpAllLeds();
void lightDownLeds(uint8_t ledbyte);
void lightDownAllLeds();
void lightUpLed(int lednumber);
void lightDownLed(int lednumber);
void lightsCountDown();