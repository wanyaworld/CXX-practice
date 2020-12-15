#include<map>
#include<iostream>
#include"myClass.h"

using namespace std;

#define N_OBJS 10

int main() {
	myClass *instance[N_OBJS];

	for (int i = 0 ; i < N_OBJS ; i++)
	  instance[i] = new myClass;
	for (int i = 0 ; i < N_OBJS ; i++)
	  printf("[%x]age is: %d\n", instance[i], (instance[i])->age);
	std::map<myClass, int, ClassCompare> mymap;


	for (int i = 0 ; i < N_OBJS ; i++)
	  mymap[*instance[i]] = instance[i]->age;

	int result = mymap[*instance[0]];

	printf("0th age is %d\n", result);
	return 0;
}



