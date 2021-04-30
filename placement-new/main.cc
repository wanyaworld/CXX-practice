#include <stdlib.h>
#include <iostream>

class MyClass {
public:
	int a, b;
	char *p;
	MyClass() { p = new char[0x100]; }
	~MyClass() { delete [] p; }
};

int main() {
	char *buf = new char[0x1000];
	MyClass *pmc = new MyClass();

	/* placement-new upon buffer */
	MyClass *pmc_place = new (buf) MyClass();

	/* manually call destructor first */
	pmc_place->~MyClass();
	
	/* then delete buffer */
	delete [] buf;
	
	delete pmc;
	
	return 0;
}
