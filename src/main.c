#include <avr/io.h>
#include <display.h>
#include <usart.h>
#include <util/delay.h>
#include <wpsh209.h>
#include <avr/interrupt.h>
#include <stdbool.h>

bool gameOver = true;
int velocity = 100;
bool gameStart = false;


void startGame(){
  if(!gameStart) {
    gameStart=true;
  }
  printf("beep noise\n");
}

ISR(PCINT1_vect) {
  if ((PINC & (1 << PC1)) == 0) {
    printf("Player 1 (left)\n");
    startGame();
  } else if ((PINC & (1 << PC2)) == 0) {
    //printf("2\n");
  } else if ((PINC & (1 << PC3)) == 0) {
    printf("Player 2 (right)\n");
    startGame();
  }
}

int main(void) {
  initUSART();
  initDisplay();
  initADC();
  initButtonISR();
  sei();
  while (1) {
    velocity = analogToDigital();
    writeNumberAndWait(velocity, 20);
    if(!gameStart) {
      printf("velocity: %d\n", velocity);
    }
  }
}