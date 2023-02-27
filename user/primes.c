#include "../kernel/types.h"
#include "user.h"
#define MSGSIZE 36
// define a sign to distinguish whether it's a or not
#define ZERO '0'
#define ONE '1'

void prime(int receive_pipe, int send_pipe)
{
    // receive the data from the last round
    char buf[MSGSIZE];
    read(receive_pipe,buf,MSGSIZE);

    int val = 0;
    for (int i = 0; i < MSGSIZE; ++i) {
        if (buf[i] == ONE) {
            val = i;
            break;
        }
    }
    // when it comes to here meaning all done
    if (val == 0) {
        return;
    }
    printf("prime: %d\n", val);
    // what have been print--prime number, we just put it into zero, so that we don't need to consider it
    buf[val] = ZERO;
    for (int i = 0; i < MSGSIZE; ++i) {
        if (i % val == 0) {
            buf[i] = ZERO;
        }
    }
    int pid = fork();
    if (pid > 0) {
        write(send_pipe, buf, MSGSIZE);
    } else {
        prime(receive_pipe, send_pipe);
    }
}


int main()
{
    int fd[2];
    pipe(fd);

    char nums[MSGSIZE];
    for (int i = 0; i < MSGSIZE; ++i) {
        nums[i] = ONE;
    }

    int pid = fork();
    if(pid>0){
        // 0 and 1 are not
        nums[0]=0;
        nums[1]=0;
        // write to the pipe so that child process can sieve it
        write(fd[1], nums, MSGSIZE);
        // wait for the child process but why?--since we can got the same result
        // wait(0);
    }
    if (pid == 0) {
        prime(fd[0], fd[1]);
    }
    wait(0);
    exit(0);
}

