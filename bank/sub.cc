#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// 시간 관리를 위한 변수들
int finished_people_num = 0, people_num = 0, waited_min = 0;

// 현재 시간 (분)
int cur_time = 0;

// 사람을 나타내는 노드
struct node {
  int start_min;
  int remaining_min;
  struct node *next;
};

// atm 기기를 나타내는 녿,
struct atm {
  struct node *head, *tail;
  int people_waiting;
};

struct atm *patms[3];

// 큐에서 삭제하는 함수
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
  if (patm->people_waiting >= 1)
    (patm->people_waiting)--;
}

// 큐에 삽입하는 함수
void insert(struct atm *patm, struct node *pnode) {
  if (!patm->tail) {
    patm->tail = pnode;
    patm->head = pnode;
  }
  else {
    patm->tail->next = pnode;
    patm->tail = pnode;
  }
  (patm->people_waiting)++;
}

void insertNode (struct node *pnode, int n) {
  insert(patms[n], pnode);
}

// 가장 적은 대기줄을 갖는 atm에 사람을 삽입하는 함수
void insertToAtm (struct node* pnode, int epoch) {
  int min_len = 1000000;
  int min_idx = 0;

  for (int i = 0 ; i < 3 ; i++) {
    if (patms[i]->people_waiting < min_len) {
      min_len = patms[i]->people_waiting;
      min_idx = i;
    }
  }

  insertNode(pnode, min_idx);
  people_num++;
  printf("%d시%d분 - %d번 고객(%d분 소요)이 %d번 ATM에서 서비스를 시작함.\n", 9 + (epoch / 60), epoch % 60, epoch, pnode->remaining_min, min_idx);
}

// 매 분마다 호출되어 총 기다린 시간을 증가시키고
// 현재 업무를 보는 사람의 소요시간을 감소시켜
// 업무를 다 보았으면 큐에서 제거하는 함수
void execute() {
  for (int i = 0 ; i < 3 ; i++) {
    struct atm *cur_atm = patms[i];
    int waiting_people_num = cur_atm->people_waiting - 1;
    if (waiting_people_num > 0) {
      waited_min += waiting_people_num;
    }
    if (cur_atm->head) {
      if ((cur_atm->head->remaining_min)-- <= 0) {
	del(cur_atm);
	finished_people_num++;
      }
    }
  }
}

// 총 기다리는 사람 수를 리턴하는 함수
int numWaiting() {
  int ret = 0;
  for (int i = 0 ; i < 3 ; i++) {
    struct atm *cur_atm = patms[i];
    int waiting_people_num = cur_atm->people_waiting - 1;
    if (waiting_people_num > 0) 
      ret += waiting_people_num;
  }
  return ret;
}


// 메인함수
int main() {
  srand(time(NULL));
  for (int i = 0 ; i < 3 ; i++)
    patms[i] = (struct atm*)malloc(sizeof(struct atm*) * 3);
  while (1) {
    cur_time++;
    if (cur_time == 120) break;
    struct node *pnode;
    pnode = (struct node*)malloc(sizeof(struct node));
    pnode->remaining_min = rand() % 9 + 2;
    pnode->start_min = cur_time;
    insertToAtm(pnode, cur_time);
    execute();
  }

  printf("총 %d 명의 사람이 업무를 보았습니다.\n", finished_people_num);
  printf("평균 %f 분을 기다렸습니다. \n", (float)waited_min / people_num);
  printf("총 %d 명이 기다리고 있습니다. \n", numWaiting());
  
  for (int i = 0 ; i < 3 ; i++) {
    struct node* n;
    for (struct node *p = patms[i]->head ; p ; p = n) {
      n = p->next;
      free (p);
    }
    free(patms[i]);
  }
  return 0;
}

