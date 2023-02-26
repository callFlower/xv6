#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#define MSGSIZE 16

char* ping = "ping";
char* pong = "pong";

int main(){
    // array used to receive the bytes
    char inbuf[MSGSIZE];
    // fd->file descriptor
    int fd[2];
    // int pipe(int fds[2]);
    pipe(fd);
    // create a child
    int pid = fork();
    // parent
    if(pid>0){
        // printf("我恁爹\n");
        // write to the pipe
        write(fd[1],ping,MSGSIZE);
        // wait for the quit of child progress
        wait(NULL);
        // read the message from the pipe to array
        read(fd[0], inbuf, MSGSIZE);
        printf("%d: received %s\n", getpid(),inbuf);
    }else{
        // printf("我恁儿\n");
        // read the message from the pipe to array
        read(fd[0], inbuf, MSGSIZE);
        printf("%d: received %s\n", getpid(),inbuf);
        // write to the pipe
        write(fd[1],pong,MSGSIZE);
    }
    return 0;
}