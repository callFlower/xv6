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
    read(0,buf,MSGSIZE);
    printf("what we got in standard input: %s", buf);

}





