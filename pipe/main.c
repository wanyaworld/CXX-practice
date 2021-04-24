#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#define NP 3

int filefd;

void loop (int pid, int dest_init_flag, char state, int c0_r, int c0_w, int c1_r, int c1_w) {
  char buf, word;
  while (1) {
    if (state == 'd') {
      printf("%d wating from right i am in %c\n", getpid(), state);
      read(c1_r, &buf, sizeof(buf)); 
    }
    else if (state == 'w' || state == 's') {
      printf("%d wating from left i am in %c\n", getpid(), state);
      read(c0_r, &buf, sizeof(buf));
    }
    else if (state == 'e' &&  dest_init_flag) {
      printf("%d wating from left i am in %c\n", getpid(), state);
      read(c0_r, &buf, sizeof(buf));
    }
    else if (state == 'e' &&  !dest_init_flag) {
      printf("%d wating from right i am in %c\n", getpid(), state);
      read(c1_r, &buf, sizeof(buf));
    }
    
    /* Keep writing */
    if (buf == 'w') {
      printf("%d GET W i am %c!!!!...\n", getpid(), state);
      assert(state == 'w');
      int is_first = 1;
      while  (1) {
        int ret = read(filefd, &word, 1);
        if (ret == 0) { 
          write(c1_w, "s", sizeof(char));
          state = 's';
          dest_init_flag = 1;
          printf("Read all data\n");
          break; 
        }
        if (is_first) {
          printf("%d ", getpid());
          is_first = 0;
        }
        printf("%c", word);
        if (word == '\n') { 
          write(c1_w, "w", sizeof(char));
          break; 
        }
      }
    }
    /* Start pending */
    else if (buf == 's') {
      printf("%d GET S i am %c!!!!...\n", getpid(), state);
      assert(state == 'w' || state == 's');
      if (state == 's') {
        write(c1_w, "e", sizeof(char));
        state = 'e';
      }
      else {
        write(c1_w, "s", sizeof(char));
        state = 's';
      }
    }
    /* Exiting */
    else if (buf == 'e') {
      printf("%d GET E i am %c!!!!...\n", getpid(), state);
      assert(state == 's' || state == 'e' || state == 'd');
      if (state == 'd') 
        return;
      if (state == 'e') {
        write(c0_w, "e", sizeof(char));
        if (pid == 0) {
          printf("%d I'm exiting...\n", getpid());
          exit(0);
        }
        else 
          state = 'd';
      }
      else {
        write(c1_w, "e", sizeof(char));
        state = 'e';
      }
    }
  }
}
  int
main(int argc, char *argv[])
{
  int p1[2];
  int p2[2];
  int c0_r;
  int c0_w;
  int c1_r;
  int c1_w;
  pid_t cpid;
  char buf;
  char word;
  /* w: write, s: start pending, e: exiting */
  char state = 'w';
  /* process with this flag set true will init the destroy phase. */
  int dest_init_flag = 0;

  if (argc != 2) {
    fprintf(stderr, "Usage: %s <string>\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  if ((filefd = open(argv[1], O_RDONLY)) == -1) {
    fprintf(stderr, "File not found");
    exit(EXIT_FAILURE);
  }

  if (pipe(p1) == -1) {
    perror("pipe");
    exit(EXIT_FAILURE);
  }
  c1_r = dup(p1[0]);
  c1_w = dup(p1[1]);

  for (int i = 0 ; i < NP ; i++) {
    pipe(p2);
    cpid = fork();
    if (cpid == -1) {
      perror("fork");
      exit(EXIT_FAILURE);
    }
    else if (cpid == 0) {
      c0_r = dup(p1[0]);
      c0_w = dup(p1[1]);
      
      c1_r = dup(p2[0]);
      c1_w = dup(p2[1]);
      
      p1[0] = p2[0];
      p1[1] = p2[1];
      /* ------------------------ */
      loop(cpid, dest_init_flag, state, c0_r, c0_w, c1_r, c1_w);
      /* ------------------------ */
    }
    else {
      p1[0] = p2[0];
      p1[1] = p2[1];
    }
  }

  c0_r = p2[0];
  c0_w = p2[1];



  int is_first = 1;
  while  (1) {
    int ret = read(filefd, &word, 1);
    if (ret == 0) { 
      write(c1_w, "s", sizeof(char));
      state = 's';
      break; 
    }
    if (is_first) {
      printf("%d  ", getpid());
      is_first = 0;
    }
    printf("%c", word);
    if (word == '\n') { 
      write(c1_w, "w", sizeof(char));
      break; 
    }
  }

  /* ------------------------ */
  loop(cpid, dest_init_flag, state, c0_r, c0_w, c1_r, c1_w);
  /* ------------------------ */

  assert(cpid != 0 && "only parent can go to here");
  int status;
  for (int i = 0 ; i < NP ; i++)
    wait(&status);
  
  printf("%d I'm exiting...\n", getpid());


  return 0;
}
