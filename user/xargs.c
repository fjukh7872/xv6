#include "kernel/types.h"
#include "kernel/param.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
    char buf[100];
    char* curr_index = buf;
    char* word_start_index = curr_index;
    char p[1];
    char* new_argv[MAXARG + 1];
    int x = 0;
    int* stat = &x;
    int i = 0;
    for (; i < argc - 1; i++){
        if (i >= MAXARG){
            printf("xargs: too many arguments\n");
            exit(1);
        }
        new_argv[i] = argv[i + 1];
    }
    while (read(0, p, 1) != 0){
        if (*p == ' ' || *p == '\n'){
            *curr_index = '\0';
            curr_index++;
            new_argv[i] = word_start_index;
            word_start_index = curr_index;
            i++;
            if (i >= MAXARG + 1){
                printf("xargs: too many arguments\n");
                exit(1);
            }
            if (*p == '\n'){
                new_argv[i] = "\0";
                if (fork() == 0){
                    exec(new_argv[0], new_argv);
                    printf("xargs: command failed\n");
                    exit(1);
                }
                wait(stat);
                curr_index = buf;
                word_start_index = curr_index;
                i = argc - 1;
            }
        }
        else{
            *curr_index = *p;
            curr_index++;
        }
        if (curr_index >= buf + 100){
            printf("xargs: input too long\n");
            exit(1);
        }
    }
    exit(0);
}