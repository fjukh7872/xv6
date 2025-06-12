#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
    int p[2];
    pipe(p);
    if (fork() == 0){
        char* ping_byte = "";
        read(p[0], ping_byte, 1);
        close(p[0]);
        printf("%d: received ping\n", getpid());
        write(p[1], ping_byte, 1);
        close(p[1]);
    }
    else{
        write(p[1], "H", 1);
        close(p[1]);
        char* pong_byte = "";
        read(p[0], pong_byte, 1);
        close(p[0]);
        printf("%d: received pong\n", getpid());
    }
    exit(0);
}