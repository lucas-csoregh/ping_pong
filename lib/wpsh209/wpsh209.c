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

void initADC(){ 
  ADMUX |= (1<<REFS0); //instellen van de reference voltage. We kiezen op deze manier voor de 5V als reference voltage
  ADCSRA |= (1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);//bepalen van de samplerate door een deelfactor in te stellen. Deelfactor hier: 128
  ADCSRA |= (1<<ADEN);//Enable de ADC
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

uint16_t analogToDigital(){
  ADCSRA |= (1<<ADSC);//start de conversie analoog -> digitaal
  loop_until_bit_is_clear(ADCSRA,ADSC);//wacht tot conversie gebeurd is
  return ADC;//lees het resultaat uit
}