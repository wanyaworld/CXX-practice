#include <stdio.h>

int zero_odd(int num) {
  int ret = num & 0x55555555;
  return ret;
}

int one_even(int num) {
  int ret = num | 0xaaaaaaaa;
  return ret;
}

int negative_even(int num) {
  int ret;
  int tmp = num & 0x55555555;
  int tmp2 = num & 0xaaaaaaaa;
  tmp |= 0xaaaaaaaa;
  tmp2 = ~tmp2;
  ret = tmp | tmp2;
  return ret;
}

int shift_right_one(int num) {
  int ret =  num >> 1;
  return ret;
}

int flag1(int num) {
  if ((num/10)%2 == 1)
    return 1;
  else
    return 0;
}

int flag2(int num) {
  if ((num/1000)%2 == 1)
    return 1;
  else
    return 0;
}

int main() {
  int student_id;
  printf("Enter your student ID:");
  scanf("%d", &student_id);

  for (int i = 0 ; i < 4 ; i++) {
    int student_id2;
    if (i != 0) student_id += 510;
    if (flag1(student_id) && flag2(student_id)) {
      student_id2 = zero_odd(student_id); 
      printf("For case A, the result is %d\n", student_id2);
    }
    else if (!flag1(student_id) && flag2(student_id))  {
      student_id2 = one_even(student_id); 
      printf("For case B, the result is %d\n", student_id2);
    }
    else if (flag1(student_id) && !flag2(student_id)) {
      student_id2 = negative_even(student_id); 
      printf("For case C, the result is %d\n", student_id2);
    }
    else if (!flag1(student_id) && !flag2(student_id)) {
      student_id2 = student_id;
      for (int j = 0 ; j < student_id % 10 ; j++)
        student_id2 = shift_right_one(student_id2); 
      printf("For case D, the result is %d\n", student_id2);
    }
  }
  return 0;
}

