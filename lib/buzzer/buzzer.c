#define __DELAY_BACKWARD_COMPATIBLE__
#include <avr/io.h>
//#include <led.h>
#include <usart.h>
#include <util/delay.h>
#include <wpsh209.h>
#include <stdbool.h>

//DURATION IS IN MILLISECONDEN
#define DURATION 250

//FREQUENCIES VAN DE VERSCHILLENDE NOTEN
#define  C5      523.250
#define  D5      587.330
#define  E5      659.250
#define  F5      698.460
#define  G5      783.990
#define  A5      880.000
#define  B5      987.770
#define  C6      1046.500

void enableBuzzer() { DDRD |= (1 << PD3); }//buzzer hangt op PD3

void buzzerOff() {
  PORTD &= ~(1 << PD3); // turn off PD3
}

void buzzerOn() {
  PORTD |= (1 << PD3); // turn on PD3
}

void playTone(float frequency, uint32_t duration) {
  uint32_t periodInMicro = (uint32_t)(1000000/frequency);//we berekenen de periode in microseconden uit de frequency
  uint32_t durationInMicro = duration * 1000; //we willen duration in microseconden
  for (uint32_t time = 0; time < durationInMicro; time += periodInMicro) {//Zie tutorial over muziek voor meer uitleg!
    PORTD &= ~(1 << PD3);//aanzetten van de buzzer
    _delay_us(periodInMicro / 2);//helft van de periode wachten
    PORTD |= (1 << PD3);//afzetten van de buzzer
    _delay_us(periodInMicro / 2);//weer helft van de periode wachten
  }
}

void youWin(int whoWon) {
  if(whoWon == 1) {
    playTone(C5, 200);
    _delay_ms(200);
    playTone(C5, 50);
    _delay_ms(50);
    playTone(C5, 50);
    _delay_ms(50);
    playTone(C5, 100);
    _delay_ms(50);
    playTone(E5, 300);
    _delay_ms(300);
  } else if(whoWon == 2) {
    playTone(E5, 200);
    _delay_ms(200);
    playTone(E5, 50);
    _delay_ms(50);
    playTone(E5, 50);
    _delay_ms(50);
    playTone(E5, 100);
    _delay_ms(50);
    playTone(C5, 300);
    _delay_ms(300);
  }
}

void player1Hit() {
  playTone(E5,10);
  _delay_ms(10);
}
void player2Hit() {
  playTone(C5,10);
  _delay_ms(10);
}

void SOS() {
  playTone(C5, 50);
  _delay_ms(50);
  playTone(C5, 50);
  _delay_ms(50);
  playTone(C5, 50);
  _delay_ms(50);

  _delay_ms(100);
  playTone(C5, 200);
  _delay_ms(100);
  playTone(C5, 200);
  _delay_ms(100);
  playTone(C5, 200);
  _delay_ms(200);

  playTone(C5, 50);
  _delay_ms(50);
  playTone(C5, 50);
  _delay_ms(50);
  playTone(C5, 50);
  _delay_ms(50);
}