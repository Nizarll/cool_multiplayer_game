#include <stdio.h>
#include <stdint.h>

int main() {
	
	uint16_t i = 2;
	uint8_t fr = i & 0xff;
	uint8_t sc = ((i << 8) & 0xff00) >> 8;
	printf("%d", fr & sc);
	return 0;
}
