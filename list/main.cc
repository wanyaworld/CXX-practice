#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

typedef struct node {
    int data;
    struct node *next;
} node;


typedef struct list {
     node *head;
     node *pos;
     int count;
} list;

typedef struct student_info {
    char name[256];
    int grade;
} student_info;

list* InitList() {
    list *l = (list *)malloc(sizeof(list));
    l->head = (node *)malloc(sizeof(node));
    l->head = NULL;
    l->pos = NULL;
    l->count = 0;
    return l;
}

int ListLen(list *l) {
    return l->count;
}

void PrintList(list *l) {
    l->pos = l->head;
    while (l && (l->pos != NULL)) {
        printf("%d -> ", l->pos->data);
        l->pos = l->pos->next;
    }
    printf("NULL\n");
}

void InsertListHead(list *l, int data) {
    node *new = (node *)malloc(sizeof(node));
    if (!new) {
        printf("Failed to create node\n");
        return;
    }

    new->data = data;
    new->next = l->head;
    l->head = new;
    ++(l->count);
}

int DeleteListHead(list *l) {
    int data = l->head->data;
     
    l->pos = l->head;
    if (l->pos != NULL) {
        l->pos = l->pos->next;
        --(l->count);
        free(l->pos);
    }
    printf("%d deleted\n", data);
    return data;
}

void DelList(list *l) {
    while (l && (l->head != NULL)) {
        l->pos = l->head;
        l->head = l->head->next;
        free(l->pos);
    }
    free(l);
}

int main(void) {
    list *l = InitList();
    student_info si[5];
    student_info min, max;
    int sum = 0;

    for (int i = 0; i< 5; i++){
        printf("student name: ");
        scanf("%s", si[i].name);
        printf("grade: ");
        scanf("%d", &(si[i].grade));
        InsertListHead(l, (int)(&(si[i])));
        if (i == 0 || si[i].grade <= min.grade) min = si[i];
        if (i == 0 || si[i].grade >= max.grade) max = si[i];
        sum += si[i].grade;
    }
    printf("max grade: %s %d point\n", max.name, max.grade);
    printf("min grade: %s %d point\n", min.name, min.grade);
    printf("average grade: %f\n", (float)sum / 5);
    return 0;
}
