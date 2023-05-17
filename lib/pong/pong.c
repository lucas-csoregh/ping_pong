#include <avr/interrupt.h>
//#include <wpsh209.h>
#include <buttons.h>
#include <adc.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <util/delay.h>
#include <display.h>

#include <pong.h>
#include <buzzer.h>
#include <clock.h>

#define P1 "Player 1"
#define P2 "Player 2"
#define PLAYER 15


int velocity = 0;

bool initGame = false; // intros roll
bool gameRunning = false; // the actual game part of the program really starts here
bool gameOver = false; // game over
bool gamePaused = false;
bool introOver = false;
char field[] = "[--------------------------------------------------]\n";

int inDeBuurt = 5;

char line = '-';
char ball = 'O';
int ballIndex = 0; // where in the char field[] is the ball rn

int whoHitLast =0;

int score[2];


int rightBorder = 1;
int leftBorder = sizeof(field) -4;

unsigned long lastTime = 0;


typedef struct Turn {
    char* player; // player who took the lucifers
    int velocity; // number of lucifers taken
    int ballIndex; // number of lucifers left after the turn
} Turn;
Turn *turns = NULL; // Create an empty array to hold your turns:
int numTurns = 0;

void newTurn(char who[], int vel, int index) {
  Turn newTurn;
  newTurn.player = who;
  newTurn.velocity = vel;
  newTurn.ballIndex = index;

  numTurns++;
  turns = (Turn*) realloc(turns, numTurns * sizeof(Turn));
  turns[numTurns-1] = newTurn;
}

void readVelocity() {
  int analog = analogToDigital();
    velocity = analog;
}

void end() {
  printf("\n\nSpelverloop:\n");
  printf("------------\n");
  for (int i = 0; i < numTurns; i++) {
    if(i == 0) {
        printf("START: %s served the ball at velocity: '%d'\n\n", turns[i].player, turns[i].velocity);
    } else {
        printf("Turn %d: %s hit the ball back at velocity: '%d' and ballindex: '%d'\n", i, turns[i].player, turns[i].velocity, turns[i].ballIndex);
    }
  }
  printf("\n");
  if (turns != NULL) { // free up the manually allocated memory block after using it to store the turns
    //printf("freeing manually allocated memory...\n");
    free(turns);
    turns = NULL; // Optional: set the pointer to NULL to avoid dangling pointer issues
  }
  //printf("FIN\n");
}

void winnerScreen(int whoWon) {

  int index = whoWon -1;

  printf("\n\nPlayer %d WON!!\n", whoWon);
  printf("--------------\n");
  for(int i=0; i<2; i++) {
    printf("Player %d score: %d\n", i+1, score[i]);
  }

  if(score[0] == 0 && score[1] == 0) {
    SOS();
  } else if(whoWon == 1) {
    youWin(1);
  } else if(whoWon == 2) {
    youWin(2);
  } 

  end();

  while(!gameOver) {
    writeLetterToSegment(0, PLAYER);
    writeNumberToSegment(1, whoWon);
    
    writeNumberToSegment(2, (score[index]/ 10) % 10);
    writeNumberToSegment(3, score[index] % 10);
  }

}

void startGame(int who){
  if(!initGame) {
    whoHitLast = who;
    initGame=true;
    gameRunning = true;
  }
}

void introMuziekske() {
  printf("Press button 1 or 3 to start the game,\npress button 2 during game to pause the game.\n\n");
  enableBuzzer();
  while(!initGame) {
    for (int note = 0; note < 8; note++) {
      introOver = true;
      if(initGame) {
        break;
      }
      //changeVelocity(analogToDigital());
      readVelocity();
      writeNumberAndWait(&velocity, 20);
      playTone(frequencies[note], DURATION);
      _delay_ms(DURATION);
    }
  }
}

void serveBall() {
  if(whoHitLast ==1 || whoHitLast==2) {
    if(whoHitLast==1) {
      field[rightBorder] = ball;
      ballIndex = rightBorder;

      newTurn(P1, velocity, ballIndex);
    } else if(whoHitLast==2) {
      field[leftBorder] = ball;
      ballIndex = leftBorder;

      newTurn(P2, velocity, ballIndex);
    }
    printf("vel:%d %s", velocity, field);
  }
}

void hitBall() {
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
        readVelocity();
        writeNumberAndWait(&velocity, 20);
    }
    printf("vel:%d %s", velocity, field);

    if(ballIndex == leftBorder || ballIndex == rightBorder) {
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

void sendBall() {
  readVelocity();
  if(ballIndex == 0) {
    serveBall();
  } else {
    hitBall();
  }
}

void button1() {
  if ((PINC & (1 << PC1)) == 0) {
    if(!gameRunning) {
      //whoHitLast = 1;
      startGame(1); // btn 1 pressed, start game as player 1
    } else if(ballIndex < rightBorder + 5) {
      player1Hit();
      whoHitLast = 1;
      score[0]++;
      newTurn(P1, velocity, ballIndex);
    }
  } 
}

void button2() {
  if ((PINC & (1 << PC2)) == 0) {
    //gameOver = true; // btn 2 pressed, game over
    if(gamePaused) {
      gamePaused = false;
    } else {
      gamePaused = true;
    }
  } 
}

void button3() {
  if ((PINC & (1 << PC3)) == 0) {
    if(!gameRunning) {
      //whoHitLast = 2;
      startGame(2); // btn 3 pressed, start game as player 2
    } else if(ballIndex > leftBorder - 5){
      player2Hit();
      whoHitLast = 2;
      score[1]++;
      newTurn(P2, velocity, ballIndex);
    }
  }
}

ISR(PCINT1_vect) {
  // FIXED: when these three if statements were in an if-elseif-else chain, player1 could sabotage player 3 by holding in their button
  // but when the conditional chain is split like this, they cannot sabotage eachother by holding their button 

  if(introOver) {
    button1();
    button2();
    button3();
  }
}