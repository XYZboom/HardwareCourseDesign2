#include "init.h"
#include "uart.h"
int main() {
	init();
	sendString("hello world!");
    return 0;
}
