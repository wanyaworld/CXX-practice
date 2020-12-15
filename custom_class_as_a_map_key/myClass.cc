#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "myClass.h"

myClass::myClass() {
  srand(time(NULL));
  name = new char[16];
  strcpy(name, "Jangwoong Kim");
  age = rand()%100;
  age += (int)this;
  age = age%100;
};
myClass::~myClass() {
  delete[] name;
}
