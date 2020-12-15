#include <string>
#include <iostream>
#include <chrono>
#include <string.h>

class DataClass {
	public:
		char *data;
		DataClass() { init(); }
		DataClass(const DataClass&) { init(); }
		~DataClass() { 
			if (data) delete[] data; }
		void init() {
			data = new char[10000]();
		}
		DataClass& operator=(const DataClass& param) {
			for (int i = 0 ; i < 100000 ; i++)
				data[i] = param.data[i];
			std::cout << "Copy assigned !!" << std::endl;
			return *this;
		}
		DataClass& operator=(DataClass&& param) {
			char *tmp = data;
			data = param.data;
			param.data = tmp;
			std::cout << "Move assigned !!" << std::endl;
			return *this;
		}
};

class MyClass {
	public:
		DataClass dc;
};

void func_lval(MyClass param) {
	MyClass lval;
	lval = (param);
}

void func_rval(MyClass param) {
	MyClass lval;
	lval = std::move(param);
}

int main() {
	{
		std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
		func_lval(MyClass());
		std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
		std::cout << "[copy] Time difference = " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << "[µs]" << std::endl;
	}
	{	
		std::chrono::steady_clock::time_point begin2 = std::chrono::steady_clock::now();
		func_rval(MyClass());
		std::chrono::steady_clock::time_point end2 = std::chrono::steady_clock::now();
		std::cout << "[move] Time difference = " << std::chrono::duration_cast<std::chrono::microseconds>(end2 - begin2).count() << "[µs]" << std::endl;
	}
	return 0;
}
