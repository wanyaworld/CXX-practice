#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char **argv)
{
    int N = (argc > 1) ? atoi(argv[1]) : 10;
    int c1_wr;
    int cN_rd;
    int p1[2];
    int p2[2];
    int pid_chk = getpid();

    if (N <= 0 || N >= 100)
        N = 10;

    pipe(p1);
    c1_wr = dup(p1[1]);
    printf("%d children\n", N);
    printf("Parent   = %d\n", pid_chk);

    for (int n = 0; n < N; n++)
    {
        int pid;
        pipe(p2);
        fflush(stdout);
        if ((pid = fork()) == 0)
        {
            close(p1[1]);
            close(p2[0]);
            int pid_sum;
            read(p1[0], &pid_sum, sizeof(pid_sum));
            pid_sum += getpid();
            write(p2[1], &pid_sum, sizeof(pid_sum));
            close(p1[0]);
            close(p2[1]);
            exit(0);
        }
        printf("Child %2d = %d\n", n+1, pid);
        pid_chk += pid;
        close(p1[0]);
        close(p1[1]);
        p1[0] = p2[0];
        p1[1] = p2[1];
    }

    cN_rd = p2[0];
    close(p2[1]);

    int pid_sum = getpid();
    write(c1_wr, &pid_sum, sizeof(pid_sum));
    close(c1_wr);
    read(cN_rd, &pid_sum, sizeof(pid_sum));
    close(cN_rd);
    printf("PID sum = %d\n", pid_sum);
    printf("PID chk = %d\n", pid_chk);

    return 0;
}
