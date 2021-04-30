#include <stdlib.h>
#include <iostream>

class MyClass {
public:
	int a, b;
	char* p;
	MyClass() { p = new char[10]; }
	~MyClass() { delete [] p; }
};

int main() {
	/* operator {new, delete} */
	char *raw = static_cast<char *> (operator new (0x1000));
	operator delete(static_cast<void *>(raw));

	/* {new, delete} operator for a single object */
	MyClass *obj = new MyClass;
	delete obj;

	/* {new, delete} operator for object array */
	MyClass *objs = new MyClass[0x1000];
	delete [] objs;

	return 0;
}
