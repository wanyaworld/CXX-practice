#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <limits.h>

#define N_ATM 3
#define TIME_MIN 2
#define TIME_MAX 10

unsigned int client_serviced = 0;
unsigned int clients = 0;
unsigned int total_time_waited = 0;

struct node {
	unsigned int time_spawned;
	unsigned int time_needed;
	struct node *next;
};

struct node* getNode (int time) {
	struct node* ret = (struct node*)malloc(sizeof(struct node));
	ret->time_needed = TIME_MIN + rand() % (TIME_MAX - TIME_MIN + 1);
	ret->time_spawned = time;
	return ret;
}

struct atm {
	struct node *head, *tail;
	unsigned int len;
};

struct atm atms[N_ATM];

void del(struct atm *patm) {
	if (!patm->head) return;
	struct node* prv_head = patm->head;
	if (patm->head != patm->tail) {
		patm->head = patm->head->next;
	}
	else {
		patm->head = patm->tail = NULL;
	}
	free (prv_head);
	if (patm->len >= 1)
		(patm->len)--;
}

void insert(struct atm *patm, struct node *pnode) {
	if (!patm->tail) {
		patm->tail = pnode;
		patm->head = pnode;
	}
	else {
		patm->tail->next = pnode;
		patm->tail = pnode;
	}
	(patm->len)++;
}

void insertNode (struct node *pnode, unsigned int n) {
	insert(&(atms[n]), pnode);
}

void insertToAtm (struct node* pnode, int epoch) {
	unsigned int min_len = UINT_MAX;
	unsigned int min_idx = 0;

	for (int i = 0 ; i < N_ATM ; i++) {
		if (atms[i].len < min_len) {
			min_len = atms[i].len;
			min_idx = i;
		}
	}

	insertNode(pnode, min_idx);
	clients++;
	printf("[%d:%d] %d-th client got serviced at %d-th atm\n", 9 + (epoch / 60), epoch % 60, epoch, min_idx);
}

void init() {
	srand(time(NULL));
}

void deinit() {
	for (int i = 0 ; i < N_ATM ; i++) {
		struct node* n;
		for (struct node *p = atms[i].head ; p ; p = n) {
			n = p->next;
			free (p);
		}
	}
}

void performJobs() {
	for (int i = 0 ; i < N_ATM ; i++) {
		struct atm *cur_atm = &atms[i];
		int waiting_clients = cur_atm->len - 1;
		if (waiting_clients > 0) {
			total_time_waited += waiting_clients;
		}
		if (cur_atm->head) {
			if ((cur_atm->head->time_needed)-- <= 0) {
				del(cur_atm);
				client_serviced++;
			}
		}
	}
}

unsigned int getWaitingClients() {
	unsigned int ret = 0;
	for (int i = 0 ; i < N_ATM ; i++) {
		struct atm *cur_atm = &atms[i];
		int waiting_clients = cur_atm->len - 1;
		if (waiting_clients > 0) 
			ret += waiting_clients;
	}
	return ret;
}


int main() {
	init();
	for (int i = 0 ; i < 60 * 2 ; i++) {
		struct node *pnode = getNode(i);
		insertToAtm(pnode, i);
		performJobs();
	}
	printf("total %d client got servied\n", client_serviced);
	printf("%d min waited for each client on average\n", total_time_waited / clients);
	printf("%d clinets are waiting currently\n", getWaitingClients());
	deinit();
	return 0;
}

