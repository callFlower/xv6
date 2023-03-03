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
    for (int i = 0; i <argc; ++i) {
        printf("xargs parameter is argv[%d]: %s\n",i,argv[i]);
    }


    exit(0);
}




