#include <pebble.h>
#include "timer.h"

#define TRUE 1
#define FALSE 0

#ifdef DEBUG 
const unsigned int RESET_SEC = 4;
const unsigned int INIT_SEC = 5;
#define POMO_MIN 1
#else 
const unsigned int RESET_SEC = 59;
const unsigned int INIT_SEC = 0;
#define POMO_MIN 25
#endif

#define SHORT_MIN 5 
#define LONG_MIN 15

int INIT_MIN;

void set_timer_mode(int id) {
  switch (id) {
    case 0:
      INIT_MIN = POMO_MIN;
      break;
    case 1:
      INIT_MIN = SHORT_MIN;
      break;
    case 2:
      INIT_MIN = LONG_MIN;
      break;
  }
}

void init_timer(pomo_t *timer, int id) {
  set_timer_mode(id);
  timer->min = INIT_MIN;
  timer->sec = INIT_SEC;
  timer->is_started = FALSE;
}

int update_timer(pomo_t *timer) {
  if (timer->min == 0 && timer->sec == 1) {
    tick_timer_service_unsubscribe();
    vibes_double_pulse();
    return TRUE;
  }

  if (timer->sec > 0)
    timer->sec--;
  else {
    timer->sec = RESET_SEC;
    timer->min--;
  }
  return FALSE;
}

int has_started(pomo_t timer) {
  return timer.is_started;
}

void set_started(pomo_t *timer, int flag) {
  timer->is_started = flag;
}

void reset_timer(pomo_t *timer) {
  timer->min = INIT_MIN;
  timer->sec = INIT_SEC;
}
