#include <avr/io.h>
#include <util/delay.h>

#define DELAY 150

void enableLed(int lednumber) {
  switch (lednumber) {
    case 1:
      DDRB |= (1 << PB2); // set PB2 as output
      break;
    case 2:
      DDRB |= (1 << PB3); // set PB3 as output
      break;
    case 3:
      DDRB |= (1 << PB4); // set PB4 as output
      break;
    case 4:
      DDRB |= (1 << PB5); // set PB5 as output
      break;
  }
}

void enableLeds(uint8_t ledbyte) {
  DDRB |= ledbyte & 0b111100; // set PB2 to PB5 as outputs
}

void enableAllLeds() {
  DDRB |= 0b111100; // set PB2 to PB5 as outputs
}

void lightDownLeds(uint8_t ledbyte) {
  PORTB |= ledbyte & 0b111100; // set PB2 to PB5 to high
}

void lightUpAllLeds() {
  PORTB &= ~0b111100; // set PB2 to PB5 to low
}

void lightUpLeds(uint8_t ledbyte) {
  PORTB &= ~(ledbyte & 0b111100); // set PB2 to PB5 to low
}

void lightDownAllLeds() {
  PORTB |= 0b111100; // set PB2 to PB5 to high
}

void lightDownLed(int lednumber) {
  switch (lednumber) {
    case 1:
      PORTB |= (1 << PB2); // set PB2 to high
      break;
    case 2:
      PORTB |= (1 << PB3); // set PB3 to high
      break;
    case 3:
      PORTB |= (1 << PB4); // set PB4 to high
      break;
    case 4:
      PORTB |= (1 << PB5); // set PB5 to high
      break;
  }
}

void lightUpLed(int lednumber) {
  switch (lednumber) {
    case 1:
      PORTB &= ~(1 << PB2); // set PB2 to low
      break;
    case 2:
      PORTB &= ~(1 << PB3); // set PB3 to low
      break;
    case 3:
      PORTB &= ~(1 << PB4); // set PB4 to low
      break;
    case 4:
      PORTB &= ~(1 << PB5); // set PB5 to low
      break;
  }
}

void lightsCountDown() {
  // Blink 4 lights 
  for(int i=0; i<3; i++) {
    lightUpAllLeds();
    _delay_ms(DELAY);
    lightDownAllLeds();
    _delay_ms(DELAY);
  }
  // Blink 3 lights (+speed)
  for(int i=0; i<3; i++) {
    lightUpLeds(0b011100);
    _delay_ms(DELAY);
    lightDownLeds(0b011100);
    _delay_ms(DELAY);
  }
  // Blink 2 lights (+speed)
  for(int i=0; i<3; i++) {
    lightUpLeds(0b001100);
    _delay_ms(DELAY);
    lightDownLeds(0b001100);
    _delay_ms(DELAY);
  }
  // Blink 1 lights (+speed)
  for(int i=0; i<3; i++) {
    lightUpLeds(0b000100);
    _delay_ms(DELAY);
    lightDownLeds(0b000100);
    _delay_ms(DELAY);
  }
}
