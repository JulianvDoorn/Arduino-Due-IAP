#include "lib.h"

int main() {
	const char* str = "Hello world!\nJulian is awesome\n";

	while (*str != '\0') {
		uart_putc(*str++);
	}

	return 0;
}

void uart_putc(char c) {
    while((UART->UART_SR & 2) == 0) { }
	UART->UART_THR = c;
}