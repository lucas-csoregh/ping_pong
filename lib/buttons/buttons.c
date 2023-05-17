#include <util/delay.h>
#include <avr/io.h>



void enableButtons() {
  DDRC &= ~(1 << PC1); // configureer PC1 als input
  PORTC |= (1 << PC1); // zet pull-up weerstand aan op PC1

  DDRC &= ~(1 << PC2);
  PORTC |= (1 << PC2);

  DDRC &= ~(1 << PC3);
  PORTC |= (1 << PC3);
}

void pressButton(int btnNr) {
  switch (btnNr) {
    case 1:
      DDRB |= (1 << PB2); // set PB2 as output
      break;
    case 2:
      DDRB |= (1 << PB3); // set PB3 as output
      break;
    case 3:
      DDRB |= (1 << PB4); // set PB4 as output
      break;
  }
}

void initButtonISR() {
  // set button pins to input mode
  DDRC &= ~((1 << PC1) | (1 << PC2) | (1 << PC3));

  // enable pull-up resistors on button pins
  //PORTC |= (1 << PC1) | (1 << PC2) | (1 << PC3);

  // enable pin change interrupts on button pins
  PCMSK1 |= (1 << PCINT9) | (1 << PCINT10) | (1 << PCINT11);

  // enable pin change interrupt on PCINT[14:8]
  PCICR |= (1 << PCIE1);
}