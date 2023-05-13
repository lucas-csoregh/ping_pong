#include <avr/io.h>
#include <display.h>
#include <usart.h>
#include <util/delay.h>
#include <wpsh209.h>
#include <avr/interrupt.h>
#include <stdbool.h>

#include <stdio.h>
#include <string.h>

int velocity = 0; // always shown on the segmented led display

bool initGame = false; // intros roll
bool gameRunning = false; // the actual game part of the program really starts here
bool gameOver = false ; // game over
char field[] = "[--------------------------------------------------]\n";

char line = '-';
char ball = 'O';
int ballIndex = 0; // where in the char field[] is the ball rn

int whoHitLast =0;

void lightsCountDown() {
  // Blink 4 lights 
  for(int i=0; i<3; i++) {
    lightUpAllLeds();
    _delay_ms(250);
    lightDownAllLeds();
    _delay_ms(250);
  }
  // Blink 3 lights (+speed)
  for(int i=0; i<3; i++) {
    lightUpLeds(0b011100);
    _delay_ms(250);
    lightDownLeds(0b011100);
    _delay_ms(250);
  }
  // Blink 2 lights (+speed)
  for(int i=0; i<3; i++) {
    lightUpLeds(0b001100);
    _delay_ms(250);
    lightDownLeds(0b001100);
    _delay_ms(250);
  }
  // Blink 1 lights (+speed)
  for(int i=0; i<3; i++) {
    lightUpLeds(0b000100);
    _delay_ms(250);
    lightDownLeds(0b000100);
    _delay_ms(250);
  }
}
void startGame(int who){
  if(!initGame) {
    if(who == 1) {
      printf("Player 1 (left)\n");
    } else if(who == 2) {
      printf("Player 2 (right)\n");
    }

    initGame=true;
    printf("beep noise\n"); // TODO: add oplopend geluid dat stopt wanneer het spel echt begint

    //lightsCountDown(); // TODO: uncomment later

    gameRunning = true;
  }
}

int rightBorder = 1;
int leftBorder = sizeof(field) -4;
void moveBall() {
  if(whoHitLast == 1) { // move ball to the RIGHT (towards player2, away from player1)

    if(ballIndex == 0) { // start w moving away from the left border if ballIndex hasn't been set yet
      field[rightBorder] = ball;
      ballIndex = rightBorder;
      printf(field); // the playing field = 50* "-" ( + 1* "[" and 1* "]" and 1* "\n")
    }

    while(whoHitLast == 1 && ballIndex != leftBorder) {
      int lastIndex = ballIndex;
      ballIndex++;
      field[ballIndex] = ball;
      field[lastIndex] = line;
      printf(field); // the playing field = 50* "-" ( + 1* "[" and 1* "]" and 1* "\n")
    }
  } else if(whoHitLast == 2) { // move ball to the LEFT (towards player1, away from player2)
  }
}

ISR(PCINT1_vect) {
  if ((PINC & (1 << PC1)) == 0) {
    if(!gameRunning) {
      startGame(1); // button 1 pressed, start game as player 1
    } else {
      whoHitLast = 1;
      moveBall();
    }

  } else if ((PINC & (1 << PC2)) == 0) {
    gameOver = true;
  } else if ((PINC & (1 << PC3)) == 0) {
    if(!gameRunning) {
      startGame(2); // button 3 pressed, start game as player 2
    } else {
      whoHitLast = 2;
      moveBall();
    }
  }
}


int main(void) {
  initUSART();
  initDisplay();
  initADC();
  initButtonISR();
  enableAllLeds();
  sei();
  while (!gameOver) { // Game loop
    int old = velocity;
    velocity = analogToDigital();
    writeNumberAndWait(velocity, 20);
    if(!initGame) {
      if(old != velocity) {
        printf("velocity: %d\n", velocity);
      }
    }
    /*
    if(gameRunning) {
      printf(field); // the playing field = 50* "-" ( + 1* "[" and 1* "]" and 1* "\n")
    }*/
  }
}