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

	/* placement-new upon buffer */
	MyClass *pmc_place = new (buf) MyClass();

	/* delete buffer first */
	delete [] buf;
	
	/* then manually call destructor */
	pmc_place->~MyClass();
	
	delete pmc;
	
	return 0;
}
