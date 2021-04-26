#include <pthread.h>
#include "peterson.h"

class PetersonSem {
public:
	PetersonSem(int n_threads) { init(n_threads); }
	void wait(int self);
	void signal(int self);
private:
	void init(int n_threads);
	void lock(int self);
	void unlock(int self);
	int n;
	int sem;
	struct peterson p;
	struct flags f;
};

void PetersonSem::wait(int self) {
	while (1) {
		lock(self);
		if (sem >= 1) {
			sem--;
			unlock(self);
			return;
		}
		unlock(self);
	}
}
	

void PetersonSem::signal(int self) {
	lock(self);
	sem++;
	unlock(self);
}

void PetersonSem::init(int n_threads) {
	n = n_threads;
	sem = 1;
	init_lock(&p, n, &f);
}


void PetersonSem::lock(int self) {
	_lock(&p, self);
} 

void PetersonSem::unlock(int self) {
	_unlock(&p, self);
}
