#include "../kernel/types.h"
#include "../kernel/param.h"
#include "../kernel/stat.h"
#include "../kernel/fs.h"
#include "user.h"
#define MSGSIZE 16

int main(int argc, char* argv[]){
    // echo hello too | xargs echo bye

    // first step--get the input hello too, which is previous command arguments
    char buf[MSGSIZE];
    // 1--stuck, why?? 0--right
    read(0,buf,MSGSIZE);
    printf("what we got in standard input: %s\n", buf);

    // second step--get the xargs parameter
    char* giaoargv[MSGSIZE];
    int giaoargc = 0;
    // put the argv parameters first
    for (int i = 0; i < argc; ++i) {
        giaoargv[giaoargc] = argv[i];
        giaoargc++;
    }
    // put the previous command parameters second
    char* p = buf;
    for (int i = 0; i < MSGSIZE; ++i) {
        // if encounter \n we need to fork it to a new process and execute both of parent and child
        if(buf[i] == '\n'){
            int pid = fork();
            if(pid==0){
                buf[i]=0;
                giaoargv[giaoargc++] = p;
                giaoargv[giaoargc++] = 0;
                exec(giaoargv[0],giaoargv);
                exit(0);
            }else{
                p = &buf[i+1];
            }
        }
    }
    // third step--use exec to execute the command
    exit(0);
}




