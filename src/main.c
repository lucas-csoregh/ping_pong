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
bool initGame = false;


void startGame(int who){
  if(!initGame) {
    if(who == 1) {
      printf("Player 1 (left)\n");
    } else if(who == 2) {
      printf("Player 2 (right)\n");
    }

    initGame=true;
    printf("beep noise\n"); // TODO: add oplopend geluid dat stopt wanneer het spel echt begint
  }
}

ISR(PCINT1_vect) {
  if ((PINC & (1 << PC1)) == 0) {
    startGame(1); // button 1 pressed, start game as player 1
  } else if ((PINC & (1 << PC2)) == 0) {
    //printf("2\n");
  } else if ((PINC & (1 << PC3)) == 0) {
    startGame(2); // button 3 pressed, start game as player 2
  }
}

void lightsCountDown() {

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
    if(!initGame) {
      printf("velocity: %d\n", velocity);
    }
  }
}