#include <thread>
#include <iostream>
#include <atomic>

int a, b;

void func_a() {
  a++;
  std::atomic_thread_fence(std::memory_order_acquire);
  if (b == 1)
    a++;
}

void func_b() {
  b++;
  std::atomic_thread_fence(std::memory_order_acquire);
  if (a == 1)
    b++;
}

int main() {
  while (1) {
    a = b = 0;
    std::thread thr_a(func_a);
    std::thread thr_b(func_b);
    thr_a.join();
    thr_b.join();
    if (a == 1 && b == 1)
      std::cout << a << " " << b << std::endl;
  }
  return 0;
}
