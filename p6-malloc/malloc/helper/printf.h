#ifndef PRINTF_H_
#define PRINTF_H_
/* printf.c */
#include "concurrency.h"

int printf_u(const char *format, ...); // unsynchronized
//int printf_m(const char *format, ...); // protected by a mutex
int printf_i(const char *format, ...); // protected by a interrupt-disable
//int printf_mi(const char *format, ...); // protected by a mutex followed by interrupt-disable
int printf(const char *format, ...); // protected by something or other
int sprintf(char *out, const char *format, ...);

#endif
