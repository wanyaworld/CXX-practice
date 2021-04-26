#include <stdio.h>

int downOdd(int n) {
	return n & 0xaaaaaaaa;
}
int upEven(int n) {
	return n | 0xaaaaaaaa;
}
int flipEven(int n) {
	int op1 = n & 0x55555555;
	int op2 = n & 0xaaaaaaaa;
	op2 = ~op2;
	return op1 | op2;
}
int shiftRight(int n) {
	return n >> 1;
}

int main() {
	int num = 20061234;
	for (int i = 0 ; i < 4 ; i++) {
		num += 510 * i;
		printf("%d\n", downOdd(num));
		printf("%d\n", upEven(num));
		printf("%d\n", flipEven(num));
		printf("%d\n", shiftRight(num));
	}
	return 0;
}
