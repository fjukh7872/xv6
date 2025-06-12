#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#define UPPER_BOUND (35)

int
main(int argc, char *argv[])
{
    int prev_fd[2];
    int next_fd[2];
    int p = 2;
    int n = 3;
    pipe(prev_fd);
    for (int i = 2; i < UPPER_BOUND; i++){
        write(prev_fd[1], &i, 4);
    }
    while(1){
        close(prev_fd[1]);
        if (read(prev_fd[0], &p, 4) == 0){
            exit(0);
        }
        printf("prime %d\n", p);
        pipe(next_fd);
        if (fork() != 0){
            close(next_fd[0]);
            while (read(prev_fd[0], &n, 4) != 0){
                if (n%p != 0){
                    write(next_fd[1], &n, 4);
                }
            }
            close(prev_fd[0]);
            close(next_fd[1]);
            wait((int*) 0);
            exit(0);
        }
        close(prev_fd[0]);
        prev_fd[0] = next_fd[0];
        prev_fd[1] = next_fd[1];
    }
    exit(0);
}