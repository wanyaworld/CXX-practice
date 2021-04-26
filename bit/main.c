#include <stdio.h>

int isOddTen(int n) {
	return (n / 10) % 2;
}

int isOddThou(int n) {
	return (n / 1000) % 2;
}

int downOdd(int n) {
	return n & 0x55555555;
}
int upEven(int n) {
	return n | 0xaaaaaaaa;
}
int flipEven(int n) {
	int op1 = n & 0x55555555;
	int op2 = n & 0xaaaaaaaa;
	op1 |= 0xaaaaaaaa;
	op1 = ~op1;
	return op1 | op2;
}
int shiftRight(int n, int shift) {
	return n >> shift;
}

int main() {
	int num = 20061234;
	for (int i = 0 ; i < 4 ; i++) {
		int print;
		char c;
		if (isOddTen(num) && isOddThou(num)) {
			c = 'A';
			print = downOdd(num); }
		else if (!isOddTen(num) && isOddThou(num))  {
			c = 'B';
			print = upEven(num); }
		else if (isOddTen(num) && !isOddThou(num)) {
			c = 'C';
			print = flipEven(num); }
		else if (!isOddTen(num) && !isOddThou(num)) {
			c = 'D';
			print = shiftRight(num, num % 10); }
		printf("For case %c, the result is %d\n", c, print);
		num += 510;
	}
	return 0;
}
