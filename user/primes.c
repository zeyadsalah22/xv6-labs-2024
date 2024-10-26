#include "user.h"

void prime_sieve(int left_fd){
    int p;
    // read the first number from the pipe
    if(read(left_fd, &p, 4)==0){
        close(left_fd);
        exit(0);
    }
    fprintf(1, "prime %d\n", p);
    // initialize a pipe for the next process
    int right_fd[2];
    pipe(right_fd);
    int pid = fork();
    if(pid){
        // parent
        close(right_fd[0]);
        int n;
        // pass all numbers that are not divisible by p to the next process
        while(read(left_fd, &n, 4)!=0){
            if(n%p!=0){
                write(right_fd[1], &n, 4);
            }
        }
        close(left_fd);
        close(right_fd[1]);
        // wait for the child process to finish
        wait(0);
    }
    else{
        // child
        close(right_fd[1]);
        // recursively call the sieve algorithm with the next process to the remaining numbers
        prime_sieve(right_fd[0]);
    }
}

int main(int argc, char* argv[]){
    int fd[2];
    pipe(fd);
    int pid = fork();
    if(pid){
        // parent
        close(fd[0]);
        // write all numbers to the pipe
        for(int i=2; i<=35; i++){
            write(fd[1], &i, 4);
        }
        close(fd[1]);
        // wait for the child process to finish
        wait(0);
    }
    else{
        // child
        close(fd[1]);
        // start sieve algorithm with the numbers from the pipe
        prime_sieve(fd[0]);
    }
     
    exit(0);
}