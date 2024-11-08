#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"
#include "kernel/param.h"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(2, "Usage: xargs <command> [args,....]\n");
        exit(1);
    }
    char *cmd = argv[1];
    //set up initial command arguments from argv
    char *args[MAXARG];
    for (int i=1; i<argc; i++) {
        args[i-1] = argv[i];
    }

    //Null-terminate the list
    args[argc-1] = 0;
    char buf[512];
    int n, i = 0;

    //read input line by line
    while ((n = read(0, buf+i, 1)) > 0) {
        if (buf[i] == '\n') {
            buf[i] = 0;
            args[argc-1] = buf;
            args[argc] = 0;
            //fork and execute command with args
            if (fork() == 0) {
                exec(cmd, args);
                fprintf(2, "exec failed\n");
                exit(0);
            }
            //parent waits for child to finish
            wait(0);
            //reset index for the next line
            i = 0;
        } else {
            i++;
            if (i >= sizeof(buf) - 1) {
                fprintf(2, "xargs: line too long\n");
                exit(1);
            }
        }
    }
    if (n<0) {
        fprintf(2, "xargs: read error\n");
        exit(1);
    }
    exit(0);
}