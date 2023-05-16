#include <util/delay.h>
#include <avr/io.h>

void enableButtons();
void enableBuzzer();
void buzzerOff();
void buzzerOn();
void playTone(float frequency, uint32_t duration);

void initADC();
void initButtonISR();
uint16_t analogToDigital();