#include <iostream>
#include <thread>

constexpr unsigned int N = 256;

class MyClass {
public:
  MyClass() {
    std::cout << "MyClass is init'ed" << std::endl;
  }
};


void func() {
  static MyClass* pmc = new MyClass;
}

int main() {
  std::thread thrs[N];
  std::cout << "Before calling func" << std::endl;
  for (int i = 0 ; i < (int)N ; i++)
    thrs[i] = std::thread (func);
  func();
  std::cout << "After calling func" << std::endl;
  for (int i = 0 ; i < (int)N ; i++)
    thrs[i].join();
  return 0;
}
