#include <util/delay.h>
#include <avr/io.h>

void enableButtons();
void enableBuzzer();
void buzzerOff();
void buzzerOn();
void playTone(float frequency, uint32_t duration);
void enableLed(int lednumber);
void enableLeds(uint8_t ledbyte);
void enableAllLeds();
void lightUpLeds(uint8_t ledbyte);
void lightUpAllLeds();
void lightDownLeds(uint8_t ledbyte);
void lightDownAllLeds();
void lightUpLed(int lednumber);
void lightDownLed(int lednumber);
void initADC();
void initButtonISR();
uint16_t analogToDigital();