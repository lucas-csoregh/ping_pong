#include <avr/io.h>
#include <util/atomic.h>

volatile unsigned long millisCount = 0;

void initTimer1() {
  TCCR1B |= (1 << WGM12);            // CTC mode
  OCR1A = F_CPU / 8000 - 1;
  TIMSK1 |= (1 << OCIE1A);           // Enable Timer1 compare match interrupt
  TCCR1B |= (1 << CS11);
}

unsigned long millis() {
  unsigned long count;
  
  // Disable interrupts temporarily
  ATOMIC_BLOCK(ATOMIC_FORCEON) {
    count = millisCount;
  }
  
  return count;
}

ISR(TIMER1_COMPA_vect) {
  millisCount++;
}