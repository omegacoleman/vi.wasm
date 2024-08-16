#ifndef __WN_H__
#define __WN_H__

#include "sys/types.h"

void wn_dbglog(const char* log);

typedef void (*wn_winch_handler_t)();

int wn_ttyread(char* ptr, int len);
int wn_ttywrite(const char* ptr, int len);
int wn_ttywait(int timeout_ms);
void wn_ttysize(int *w, int *h);

int wn_kvpread(const char* key, off_t off, char* ptr, int len);
int wn_kvpwrite(const char* key, off_t off, const char* ptr, int len);
off_t wn_kvlen(const char* key);
int wn_kvsetlen(const char* key, off_t len);

void wn_set_winch_handler(wn_winch_handler_t func);

#endif

