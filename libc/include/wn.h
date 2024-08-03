#ifndef __WN_H__
#define __WN_H__

typedef void (*wn_winch_handler_t)();

int wn_ttywait(int timeout_ms);
void wn_ttysize(int *w, int *h);
void wn_set_winch_handler(wn_winch_handler_t func);

#endif

