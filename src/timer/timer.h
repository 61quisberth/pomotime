#ifndef TIMER_H
#define TIMER_H

typedef struct pomo_t {
  unsigned int min;
  unsigned int sec;
  int is_started;
} pomo_t;

void init_timer(pomo_t *timer, int id);
int update_timer(pomo_t *timer);
void reset_timer(pomo_t *timer);
int has_started(pomo_t timer);
void set_started(pomo_t *timer, int flag);

#endif
