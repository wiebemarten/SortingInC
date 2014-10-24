#ifndef TIMER_H
#define TIMER_H

#include <sys/time.h>

typedef struct Timer {
  struct timeval start;
  struct timeval stop;
} Timer;

void startTimer(Timer *timer);
void stopTimer(Timer *timer);
double seconds(Timer *timer);
int milliseconds(Timer *timer);

#endif
