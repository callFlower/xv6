#include "kernel/types.h"
#include "kernel/stat.h"
#include "user.h"

int main(int argc, char *argv[])
{
    // sleep->argv[0]  100(the number)->argv[1]  ==> argc is 2
    if(argc!=2){
        // fprintf is a C function to print a formatted string to a file. 2->error
        // fp = fopen ("file.txt", "w+");
        // fprintf(fp, "%s %s %s %d", "We", "are", "in", 2014);
        fprintf(2,"Usage: sleep <time>\n");
        exit(1);
    }
    else{
        sleep(atoi(argv[1]));
        exit(0);
    }
}
