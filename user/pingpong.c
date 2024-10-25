#include "user/user.h"

int main(int argc, char* argv[])
{
    int fd1[2];
    int fd2[2];
    if(pipe(fd1)==-1 || pipe(fd2)==-1){
        fprintf(2, "pipe failed\n");
        exit(1);
    }
    int pid = fork();
    if(pid==0){
        // Child
        close(fd1[1]);
        close(fd2[0]);
        char c;
        //read from pipe 1
        if(read(fd1[0], &c, 1)==-1){
            printf("read failed\n");
            exit(1);
        }
        fprintf(1, "%d: received ping\n", getpid());
        // write to pipe 2
        if(write(fd2[1], &c, 1)==-1){
            printf("write failed\n");
            exit(1);
        }
        close(fd1[0]);
        close(fd2[1]);
        exit(0);
    }
    else{
        // Parent
        close(fd1[0]);
        close(fd2[1]);
        char c = 'a';
        // write to pipe 1
        if(write(fd1[1], &c, 1)==-1){
            printf("write failed\n");
            exit(1);
        }
        // read from pipe 2
        if(read(fd2[0], &c, 1)==-1){
            printf("read failed\n");
            exit(1);
        }
        fprintf(1, "%d: received pong\n", getpid());
        close(fd1[1]);
        close(fd2[2]);
    }
    exit(0);
}