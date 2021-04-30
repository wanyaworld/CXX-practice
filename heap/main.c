#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define MEM_SIZE 256
#define MAX_OBJ MEM_SIZE/sizeof(int)

struct metadata {
  char data_name[256];
  unsigned int pos;
  unsigned int size;
};

struct memory_manager {
  void *start;
  unsigned int max_size;
  unsigned int cur_size;
  unsigned int cur_objs;
  struct metadata md_list[MAX_OBJ];
};

struct memory_manager mm;

char string_question_type[256] = "Do you want to \
allocate data (1) or deallocate data(2) ?";
char string_question_req[256] = "What is the type\
 of data you want to allocate and what will be th\
e name of the data?";
char string_question_struct_size[256] = "How many\
 data should be in the struct";
char string_question_struct_detail[256] = "Please\
 specify each type and its value";
char string_question_val_detail[256] = "Please\
 specify each type and its value";
char string_noti_success[256] = "has been allocat\
ed successfully";
char string_noti_success_de[256] = "has been deal\
located successfully";
char string_result[256] = "Here is the mem dump!";
char string_dealloc_name[256] = "What is the name\
of data you want to deallocate?";
char string_memory_full[256] = "There is not enou\
gh memory for the data, you can only use %d byte(\
s)";

void init() {
  mm.start = malloc(MEM_SIZE);
  mm.max_size = MEM_SIZE;
  mm.cur_size = 0;
  mm.cur_objs = 0;
};

int insert_metadata(char* name, unsigned int pos, unsigned int size) {
  mm.md_list[mm.cur_objs].pos = pos;
  mm.md_list[mm.cur_objs].size = size;
  strcpy(mm.md_list[mm.cur_objs].data_name, name);
  mm.cur_objs++;
}

int delete_metadata(char* name, unsigned int* ppos, int *size) {
  for (int i = 0 ; i < mm.cur_objs ; i++) {
    if (!strcmp(name, mm.md_list[i].data_name)) {
      *ppos = mm.md_list[i].pos;
      *size = mm.md_list[i].size;
      for (int j = i ; j + 1 < mm.cur_objs ; j++) {
        mm.md_list[j] = mm.md_list[j + 1];
      }
      (mm.cur_objs)--;
      return 1;
    }
  }
  return 0;
}

void dump_mem(const void *mem, size_t len) {
  const char *buffer = mem;
  size_t i;
  for (i = 0 ; i < len ; i++) {
    if (i > 0 && i % 16 == 0) {
      printf("\n");
    }
    printf("%02x ", buffer[i] & 0xff);
  }
  puts("");
}

int mem_available(int size) {
  if (mm.max_size - mm.cur_size >= size) {
    return 1;
  }
  printf(string_memory_full, mm.max_size);
  printf("\n");
  return 0;
}

char alloc_char(char val, char* data_name) {
  if (!mem_available(sizeof(val))) return 0;
  void *addr = (void*)((unsigned long long)mm.cur_size + (unsigned long long)mm.start);
  *(char*)addr = val;
  insert_metadata(data_name, mm.cur_size, sizeof(char));
  mm.cur_size += sizeof(val);
}

int alloc_int(int val, char* data_name) {
  if (!mem_available(sizeof(val))) return 0;
  void *addr = (void*)((unsigned long long)mm.cur_size + (unsigned long long)mm.start);
  *(int*)addr = val;
  insert_metadata(data_name, mm.cur_size, sizeof(int));
  mm.cur_size += sizeof(val);
}

int alloc_double(double val, char* data_name) {
  if (!mem_available(sizeof(val))) return 0;
  void *addr = (void*)((unsigned long long)mm.cur_size + (unsigned long long)mm.start);
  *(double*)addr = val;
  insert_metadata(data_name, mm.cur_size, sizeof(double));
  mm.cur_size += sizeof(val);
}

void dealloc_char(unsigned int p) {
  mm.cur_size -= sizeof(char);
  unsigned long long pos = (unsigned long long)p;
  void *dest = (void*)(pos + (unsigned long long)mm.start);
  void *src = (void*)(pos + (unsigned long long)mm.start + sizeof(char));
  void *rem = (void*)((unsigned long long)mm.start + mm.cur_size);
  memmove(dest, src, mm.max_size - sizeof(char));
  memset(rem, 0, sizeof(char));
}

void dealloc_int(unsigned int p) {
  mm.cur_size -= sizeof(int);
  unsigned long long pos = (unsigned long long)p;
  void *dest = (void*)(pos + (unsigned long long)mm.start);
  void *src = (void*)(pos + (unsigned long long)mm.start + sizeof(int));
  void *rem = (void*)((unsigned long long)mm.start + mm.cur_size);
  memmove(dest, src, mm.max_size - sizeof(int));
  memset(rem, 0, sizeof(int));
}

void dealloc_double(unsigned int p) {
  mm.cur_size -= sizeof(double);
  unsigned long long pos = (unsigned long long)p;
  void *dest = (void*)(pos + (unsigned long long)mm.start);
  void *src = (void*)(pos + (unsigned long long)mm.start + sizeof(double));
  void *rem = (void*)((unsigned long long)mm.start + mm.cur_size);
  memmove(dest, src, mm.max_size - sizeof(double));
  memset(rem, 0, sizeof(double));
}

int main() {
  init();
  while (1) {
    int req_type, size, val_int;
    double val_double;
    char val_char;
    char data_name[256], type_name[256];
    printf("%s\n", string_question_type);
    scanf("%d", &req_type);
    if (req_type == 1) {
      printf("%s\n", string_question_req);
      scanf("%s %s", type_name, data_name);
      if (!strcmp(type_name, "struct")) {
        printf("%s\n", string_question_struct_size);
        scanf("%d", &size);
        for (int i = 0 ; i < size ; i++) {
          printf("%s\n", string_question_struct_detail);
          scanf("%s", type_name);
          if (!strcmp(type_name, "int")) {
            scanf("%d", &val_int);
            if (!alloc_int(val_int, data_name)) continue;
          }
          else if (!strcmp(type_name, "double")) {
            scanf("%lf", &val_double);
            if (!alloc_double(val_double, data_name)) continue;
          }
          else if (!strcmp(type_name, "char")) {
            scanf("%d", &val_int);
            val_char = (char)val_int;
            if (!alloc_char(val_char, data_name)) continue;
          }
        }
      }
      else if (!strcmp(type_name, "int")) {
        printf("%s\n", string_question_val_detail);
        scanf("%d", &val_int);
        if (!alloc_int(val_int, data_name)) continue;
      }
      else if (!strcmp(type_name, "double")) {
        printf("%s\n", string_question_val_detail);
        scanf("%lf", &val_double);
        if (!alloc_double(val_double, data_name)) continue;
      }
      else if (!strcmp(type_name, "char")) {
        printf("%s\n", string_question_val_detail);
        scanf("%d", &val_int);
        val_char = (char)val_int;
        if (!alloc_char(val_char, data_name)) continue;
      }
      printf("%s %s\n", data_name, string_noti_success);
      printf("%s\n", string_result);
      dump_mem(mm.start, mm.max_size);
    }
    else if (req_type == 2){
      printf("%s\n", string_dealloc_name);
      scanf("%s", data_name);
      unsigned int pos;
      int is_first = 1;
      while (1) {
        if (!delete_metadata(data_name, &pos, &size)) {
          if (is_first)
            printf("%s has never been allocated\n", data_name);
          continue;
        }
        is_first = 0;
        if (size == sizeof(int)) {
          dealloc_int(pos);
        }
        else if (size == sizeof(double)) {
          dealloc_double(pos);
        }
        else if (size == sizeof(char)) {
          dealloc_char(pos);
        }
        else {
          printf("int nor double allocated\n");
          continue;
        }
        printf("%s %s\n", data_name, string_noti_success_de);
        printf("%s\n", string_result);
        dump_mem(mm.start, mm.max_size);
      }
    }
    else {
      printf("Invalid req type\n");
    }
  }
  return 0;
}
