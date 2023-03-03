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

    // second step--get the xargs parameter
    char* giaoargv[MAXARG];
    int giaoargc = 0;
    // put the argv parameters first
    // since we don't need xargs, only want the command after, so start from i=1
    for (int i = 1; i < argc; ++i) {
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
                // joint the parameters
                buf[i]='\0';
                giaoargv[giaoargc++] = p;
                giaoargv[giaoargc++] = '\0';
                exec(giaoargv[1],giaoargv);
            }else{
                p = &buf[i+1];
                wait(0);
            }
        }
    }
    wait(0);
    // third step--use exec to execute the command
    exit(0);
}




