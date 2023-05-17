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

#include <buzzer.h>
#include <led.h>

#define PLAYER 15

int velocity = 0; // always shown on the segmented led display

bool initGame = false; // intros roll
bool gameRunning = false; // the actual game part of the program really starts here
bool gameOver = false; // game over
bool gamePaused = false;
bool introOver = false;
char field[] = "[--------------------------------------------------]\n";

char line = '-';
char ball = 'O';
int ballIndex = 0; // where in the char field[] is the ball rn

int whoHitLast =0;

int score[2];

volatile unsigned long millisCount = 0;

int rightBorder = 1;
int leftBorder = sizeof(field) -4;

void readVelocity() {
  int analog = analogToDigital();
    velocity = analog;
  /*
  if(analog % 2 == 0 && analog != 0) {
    velocity = analog;
  }*/
  //return velocity;
}

ISR(TIMER1_COMPA_vect) {
  millisCount++;
}

void winnerScreen(int whoWon) {

  int index = whoWon -1;

  printf("\n\nPlayer %d WON!!\n", whoWon);
  printf("--------------\n");
  printf("Score: %d\n", score[index]);

  youWin();

  while(!gameOver) {
    writeLetterToSegment(0, PLAYER);
    writeNumberToSegment(1, whoWon);
    
    writeNumberToSegment(2, (score[index]/ 10) % 10);
    writeNumberToSegment(3, score[index] % 10);
  }

}

unsigned long millis() {
  unsigned long count;
  
  // Disable interrupts temporarily
  ATOMIC_BLOCK(ATOMIC_FORCEON) {
    count = millisCount;
  }
  
  return count;
}


void startGame(int who){
  if(!initGame) {
    if(who == 1) {
      printf("Player 1 (left)\n");
    } else if(who == 2) {
      printf("Player 2 (right)\n");
    }
    whoHitLast = who;

    initGame=true;

    gameRunning = true;
  }
}


ISR(PCINT1_vect) {
  // FIXED: when these three if statements were in an if-elseif-else chain, player1 could sabotage player 3 by holding in their button
  // but when the conditional chain is split like this, they cannot sabotage eachother by holding their button 

  if(introOver) {
    if ((PINC & (1 << PC1)) == 0) {
      if(!gameRunning) {
        //whoHitLast = 1;
        startGame(1); // btn 1 pressed, start game as player 1
      } else if(ballIndex < rightBorder + 5) {
        whoHitLast = 1;
        score[0]++;
      }
    } 
    if ((PINC & (1 << PC2)) == 0) {
      //gameOver = true; // btn 2 pressed, game over
      if(gamePaused) {
        gamePaused = false;
      } else {
        gamePaused = true;
      }
    } 
    if ((PINC & (1 << PC3)) == 0) {
      if(!gameRunning) {
        //whoHitLast = 2;
        startGame(2); // btn 3 pressed, start game as player 2
      } else if(ballIndex > leftBorder - 5){
        whoHitLast = 2;
        score[1]++;
      }
    }
  }
}

void initTimer1() {
  TCCR1B |= (1 << WGM12);            // CTC mode
  OCR1A = F_CPU / 8000 - 1;
  TIMSK1 |= (1 << OCIE1A);           // Enable Timer1 compare match interrupt
  TCCR1B |= (1 << CS11);
}

void introMuziekske() {
  enableBuzzer();
  while(!initGame) {
    for (int note = 0; note < 8; note++) {
      introOver = true;
      if(initGame) {
        break;
      }
      //changeVelocity(analogToDigital());
      readVelocity();
      writeNumberAndWait(velocity, 20);
      playTone(frequencies[note], DURATION);
      _delay_ms(DURATION);
    }
  }
}

void initGameReq() {
  initUSART();
  initDisplay();
  initADC();
  initButtonISR();
  enableAllLeds();
  initTimer1();
  sei();
  lightsCountDown();
  introMuziekske();
}

/*
void changeVelocity(int vel) {
  if(vel > 1) {
    velocity = vel;
  }
}*/

void sendBall() {
  if(ballIndex == 0 && (whoHitLast ==1 || whoHitLast==2)) {
    if(whoHitLast==1) { // start w moving away from the LEFT border if ballIndex hasn't been set yet
      field[rightBorder] = ball;
      ballIndex = rightBorder;
    } else if(whoHitLast==2) { // start w moving away from the LEFT border if ballIndex hasn't been set yet
      field[leftBorder] = ball;
      ballIndex = leftBorder;
    }
    //printf(field);
    printf("vel:%d %s", velocity, field);
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

    while(gamePaused || !(millis() % velocity == 0)) {
      //printf("velocity == 0: \n", velocity == 0);
      /*
      if(velocity == 0) {
        break;
      }*/
      // wait
      //changeVelocity(analogToDigital());
      readVelocity();
      //velocity = analogToDigital();
      writeNumberAndWait(velocity, 20);
    }
    //printf("%ld | %d @ %s",millis() , velocity, field);
    printf("vel:%d %s", velocity, field);

    if(ballIndex == leftBorder || ballIndex == rightBorder) {
      // TODO: add overwinningsgeluid here
      if(ballIndex == leftBorder) {
        winnerScreen(1);
      } else if(ballIndex == rightBorder) {
        winnerScreen(2);
      }


      gameOver = true;
      break;
    }
  }
}


void gameLoop() {
  int old = velocity;
  //velocity = analogToDigital();
  //changeVelocity(analogToDigital());
  readVelocity();
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