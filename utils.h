#include <stdlib.h>
#include <stdio.h>

char* intToString(int n) {
	char* str = (char*)malloc(sizeof(char) * 50);
	sprintf(str, "%d", n);
	return str;
}

char* doubleToString(double d) {
	char* str = (char*)malloc(sizeof(char) * 50);
	sprintf(str, "%lf", d);
	return str;
}

void uart_logln(char* str) {
	UART2_SendStringNewLine(str);
}

void uart_log(char* str) {
	UART2_SendString(str);
}