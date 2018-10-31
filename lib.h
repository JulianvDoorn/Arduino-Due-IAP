#ifndef LIB_H
#define LIB_H

#include "sam3x8e.h"

int main();

inline void __attribute__((always_inline)) uart_putc(char c);

#endif // LIB_H
