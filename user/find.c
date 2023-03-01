#include "../kernel/types.h"
#include "../kernel/stat.h"
#include "user.h"
#include "../kernel/fs.h"

/* In ls.c file . means current path */

//
char*
fmtname(char *path)
{
    static char buf[DIRSIZ+1];
    char *p;

    // Find first character after last slash.
    for(p=path+strlen(path); p >= path && *p != '/'; p--)
        ;
    p++;

    // Return blank-padded name.
    if(strlen(p) >= DIRSIZ)
        return p;
    memmove(buf, p, strlen(p));
    memset(buf+strlen(p), ' ', DIRSIZ-strlen(p));
    return buf;
}


void
find(char* path, char* target)
{
    // test why bug happen
    printf("path: %s; target: %s\n", path,target);

    // construct pipe
    char buf[512], *p;
    int fd;
    struct dirent de;
    struct stat st;

    // try to open the file path, if it cannot open, then  print error
    if((fd = open(path, 0)) < 0){
        fprintf(2, "ls: cannot open %s\n", path);
        return;
    }
    // don't know what fstate is, but when we open it correctly, we put/load the information from fd to &st(state)
    if(fstat(fd, &st) < 0){
        fprintf(2, "ls: cannot stat %s\n", path);
        close(fd);
        return;
    }
    // we need to make a judgement about whether the path equals to the target or not
    if(strcmp(path,target)==0){
        // if they are the same, we print the path to the console
        printf("%s\n", path);
    }
    // make a judgement to st, whether it's a file or a folder
    switch(st.type){
        // only file
        case T_FILE:
            break;

        case T_DIR:
            if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
                printf("ls: path too long\n");
                break;
            }
            strcpy(buf, path);
            p = buf+strlen(buf);
            *p++ = '/';
            // inspect all the file
            while(read(fd, &de, sizeof(de)) == sizeof(de)){
                if(de.inum == 0)
                    continue;
                memmove(p, de.name, DIRSIZ);
                p[DIRSIZ] = 0;
                if(stat(buf, &st) < 0){
                    printf("ls: cannot stat %s\n", buf);
                    continue;
                }
                if(strcmp(buf,target)==0){
                    // if they are the same, we print the path to the console
                    printf("%s\n", buf);
                }
                // printf("%s %d %d %d\n", fmtname(buf), st.type, st.ino, st.size);
                // printf("buf: %s, fmtname(buf):%s %d %d %l\n", buf,fmtname(buf), st.type, st.ino, st.size);
            }
            break;
    }
    close(fd);
}

int main(int argc, char *argv[])
{
    if(argc == 1){
        printf("usage: find [path] [target]\n");
        exit(0);
    }
    // find needs 2 parameters, 1-file path; 2-target. eg: find . a
    if(argc == 2){
        find(".",argv[1]);
        exit(0);
    }
    if(argc == 3){
        find(argv[1],argv[2]);
        exit(0);
    }
    exit(0);
}
