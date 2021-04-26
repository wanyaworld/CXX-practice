#include <pthread.h>

class PetersonSem {
public:
	PetersonSem(int n_threads) { init(n_threads); }
	void wait(int self);
	void signal(int self);
private:
	void init(int n_threads);
	void lock(int self);
	void unlock(int self);
	void lock();
	void unlock();
	pthread_mutex_t pm;
	int sem;
	int n;
	int flag[100];
	int turn;
};

void PetersonSem::wait(int self) {
	while (1) {
		lock();
		//lock(self);
		if (sem >= 1) {
			sem--;
			unlock();
			//unlock(self);
			return;
		}
		unlock();
		//unlock(self);
	}
}
	

void PetersonSem::signal(int self) {
	lock();
	//lock(self);
	sem++;
	unlock();
	//unlock(self);
}

void PetersonSem::init(int n_threads) {
	for (int i = 0 ; i < n ; i++)
		flag[i] = 0;
	turn = 0;
	n = n_threads;
	sem = 1;
}


void PetersonSem::lock(int self) {
	if(self <=n-1 && self >=0)
	{
		flag[self] = 1;
	} 
	
	turn = n-self; 
	
	if(n-1-self != self)
	{
		while (flag[n-1-self]==1 && turn==n-self) ; 

	}	
	else if(n-2-self >= 0)
	{
		while (flag[n-2-self]==1 && turn==n-self) ;
	}
} 

void PetersonSem::unlock(int self) {
	if(self <=n && self >=0)
	{
		flag[self] = 0;
	}
}

void PetersonSem::lock() {
	pthread_mutex_lock(&pm);
}
void PetersonSem::unlock() {
	pthread_mutex_unlock(&pm);
}
