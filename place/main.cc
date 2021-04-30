#include <stdlib.h>
#include <iostream>

class MyClass {
public:
	int a, b;
	~MyClass() {}
};

int main() {
	char *buf = new char[0x1000];
	MyClass *pmc = new MyClass();
	MyClass *pmc_place = new (buf) MyClass();
	delete [] buf;
	delete pmc;
	pmc_place->~MyClass();
	return 0;
}
