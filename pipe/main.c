#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>

#define NP 2

  int
main(int argc, char *argv[])
{
  int filefd;
  int p1[2];
  int p2[2];
  int c1_r;
  int c1_w;
  int cn_r;
  int cn_w;
  pid_t cpid;
  char buf;
  char word;
  /* w: write, s: start pending, e: exiting */
  char state = 'w';

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
      while (1) {
        read(p1[0], &buf, sizeof(buf));
        /* Keep writing */
        if (buf == 'w') {
          assert(state == 'w');
          while  (1) {
            int ret = read(filefd, &word, 1);
            if (ret == EOF) { 
              assert(0);
              write(p2[1], "s", sizeof(char));
              state = 's';
              break; 
            }
            printf("%c", word);
            if (word == '\n') { 
              write(p2[1], "w", sizeof(char));
              break; 
            }
          }
        }
        /* Start pending */
        else if (buf == 's') {
          assert(state == 'w' || state == 's');
          if (state == 's') {
            write(p2[1], "e", sizeof(char));
            state = 'e';
          }
          else {
            write(p2[1], "s", sizeof(char));
            state = 's';
          }
        }
        /* Exiting */
        else if (buf == 'e') {
          assert(state == 's' || state == 'e');
          if (state == 'e') {
            //write(p2[1], "e", sizeof(char));
            //exit(0);
          }
          else {
            write(p2[1], "e", sizeof(char));
            state = 'e';
          }
        }
      }
    }
    else {
      p1[0] = p2[0];
      p1[1] = p2[1];
    }
  }

  cn_r = p2[0];
  cn_w = p2[1];

  for (int i = 0 ; ; i++) {
    if (i == 0) {
      while  (1) {
        int ret = read(filefd, &word, 1);
        if (ret == EOF) { 
          write(c1_w, "s", sizeof(char));
          state = 's';
          break; 
        }
        printf("%c", word);
        if (word == '\n') { 
          write(c1_w, "w", sizeof(char));
          break; 
        }
      }
    }
    read(cn_r, &buf, sizeof(buf));
    /* Keep writing */
    if (buf == 'w') {
      assert(state == 'w');
      while  (1) {
        int ret = read(filefd, &word, 1);
        if (ret == EOF) { 
          write(c1_w, "s", sizeof(char));
          state = 's';
          break; 
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
      assert(state == 's' || state == 'e');
      if (state == 'e') {
        //write(c1_w, "e", sizeof(char));
        //exit(0);
      }
      else {
        write(c1_w, "e", sizeof(char));
        state = 'e';
      }
    }
  }








  while (1) {
    read(cn_r, &buf, sizeof(buf));
    close(filefd);

  }
  return 0;
}
