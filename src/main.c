#include <usart.h>
#include <display.h>
#include <led.h>
#include <pong.h>

#include <avr/interrupt.h>
#include <wpsh209.h>

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

void gameLoop() {
  int old = velocity;
  readVelocity();
  writeNumberAndWait(&velocity, 20);
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