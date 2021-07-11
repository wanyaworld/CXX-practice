#include <iostream>
#include <thread>
#include <assert.h>
#include <unistd.h>
#include <stdlib.h>
#include <malloc.h>
#include <linux/futex.h>
#include <sys/syscall.h>

int futex(int* uaddr, int futex_op, int val, const struct timespec* timeout,
		int* uaddr2, int val3) {
	return syscall(SYS_futex, uaddr, futex_op, val, timeout, uaddr2, val3);
}

int *buffer;

void waiter() {
	std::cout << "calling futex wait" << std::endl;
	int* futex_addr = buffer + 1;
	int ret = futex(futex_addr, FUTEX_WAIT, 1, NULL, NULL, 0);
}

void waker() {
	int* futex_addr = buffer;
	int ret = futex(futex_addr, FUTEX_WAKE, 1, NULL, NULL, 0);
	std::cout << "[waker] i wokeup " << ret << "threads" << std::endl;
}

int main() {
	int ret = posix_memalign((void**)&buffer,
			getpagesize(), getpagesize());
	assert(!ret);
	int* futex_addr = buffer + 1;
	*futex_addr = 1;
	std::thread waiter_thread(waiter);
	sleep(1);
	std::thread waker_thread(waker);
	waiter_thread.join();
	waker_thread.join();
	return 0;
}
