#include <avr/io.h>
#include <display.h>
#include <usart.h>
#include <wpsh209.h>
#include <avr/interrupt.h>
#include <stdbool.h>

#include <stdio.h>
#include <string.h>

#include <util/delay.h>
#include <util/atomic.h>

int velocity = 0; // always shown on the segmented led display

bool initGame = false; // intros roll
bool gameRunning = false; // the actual game part of the program really starts here
bool gameOver = false ; // game over
char field[] = "[--------------------------------------------------]\n";

char line = '-';
char ball = 'O';
int ballIndex = 0; // where in the char field[] is the ball rn

int whoHitLast =0;

int score[2];

volatile unsigned long millisCount = 0;

ISR(TIMER1_COMPA_vect) {
  millisCount++;
}

unsigned long millis() {
  unsigned long count;
  
  // Disable interrupts temporarily
  ATOMIC_BLOCK(ATOMIC_FORCEON) {
    count = millisCount;
  }
  
  return count;
}


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

ISR(PCINT1_vect) {
  if ((PINC & (1 << PC1)) == 0) {

    if(!gameRunning) {
      whoHitLast = 1;
      startGame(1); // btn 1 pressed, start game as player 1
    } else if(ballIndex < rightBorder + 5) {
      whoHitLast = 1;
      score[0]++;
    }
  } else if ((PINC & (1 << PC2)) == 0) {
    gameOver = true; // btn 2 pressed, game over
  } else if ((PINC & (1 << PC3)) == 0) {
    if(!gameRunning) {
      whoHitLast = 2;
      startGame(2); // btn 3 pressed, start game as player 2
    } else if(ballIndex > leftBorder - 5){
      whoHitLast = 2;
      score[1]++;
    }
  }
}

void initTimer1() {
  TCCR1B |= (1 << WGM12);            // CTC mode
  OCR1A = F_CPU / 8000 - 1;
  TIMSK1 |= (1 << OCIE1A);           // Enable Timer1 compare match interrupt
  TCCR1B |= (1 << CS11);
}

void initGameReq() {
  initUSART();
  initDisplay();
  initADC();
  initButtonISR();
  enableAllLeds();
  initTimer1();
  sei();
}

void sendBall() {
  if(ballIndex == 0 && (whoHitLast ==1 || whoHitLast==2)) {
    if(whoHitLast==1) { // start w moving away from the LEFT border if ballIndex hasn't been set yet
      field[rightBorder] = ball;
      ballIndex = rightBorder;
    } else if(whoHitLast==1) { // start w moving away from the LEFT border if ballIndex hasn't been set yet
      field[leftBorder] = ball;
      ballIndex = leftBorder;
    }
    printf(field);
  }

  while( !gameOver && (whoHitLast == 1 && ballIndex != leftBorder) || (whoHitLast == 2 && ballIndex != rightBorder)) {
    int lastIndex = ballIndex;

    if(whoHitLast == 1) {
      ballIndex++;
    } else {
      ballIndex--;
    }

    field[ballIndex] = ball;
    field[lastIndex] = line;

    //printf("%ld | %d @ %s",millis() , velocity, field);
    printf(field);

    if(ballIndex == leftBorder || ballIndex == rightBorder) {
      if(ballIndex == leftBorder) {
        printf("Player 1 WON!!\n");
      } else if(ballIndex == rightBorder) {
        printf("Player 2 WON!!\n");
      }
      printf("Score: %d\n", score[0]);
      gameOver = true;
    }
  }
}


void gameLoop() {
  int old = velocity;
  velocity = analogToDigital();
  writeNumberAndWait(velocity, 20);
  if(!initGame) {
    if(old != velocity) {
      printf("velocity: %d\n", velocity);
    }
  }

  sendBall();
}

int main(void) {
  initGameReq();
  while (!gameOver) { // Game loop
    gameLoop(); 
  }
}