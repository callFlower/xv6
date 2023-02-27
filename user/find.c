#include "../kernel/types.h"
#include "../kernel/stat.h"
#include "user.h"
#include "../kernel/fs.h"

/* In ls.c file, . means current path */

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
ls(char *path)
{
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
    // make a judgement to st, whether it's a file or a folder
    switch(st.type){
        // only file
        case T_FILE:
            // print directly
            // printf("%s %d %d %l\n", fmtname(path), st.type, st.ino, st.size);
            // test fmtname function, see what have changed
            printf("path: %s, fmtname(path):%s %d %d %l\n", path,fmtname(path), st.type, st.ino, st.size);
            break;

        case T_DIR:
            if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
                printf("ls: path too long\n");
                break;
            }
            strcpy(buf, path);
            p = buf+strlen(buf);
            *p++ = '/';
            while(read(fd, &de, sizeof(de)) == sizeof(de)){
                if(de.inum == 0)
                    continue;
                memmove(p, de.name, DIRSIZ);
                p[DIRSIZ] = 0;
                if(stat(buf, &st) < 0){
                    printf("ls: cannot stat %s\n", buf);
                    continue;
                }
                // printf("%s %d %d %d\n", fmtname(buf), st.type, st.ino, st.size);
                printf("buf: %s, fmtname(buf):%s %d %d %l\n", buf,path,fmtname(buf), st.type, st.ino, st.size);
            }
            break;
    }
    close(fd);
}

int main(int argc, char *argv[])
{
    int i;
    // receiving the parameter. eg: ls a: ls-first, a-second
    // if only ls, then ls .
    if(argc < 2){
        ls(".");
        exit(0);
    }
    // if is not, looping read parameter and execute. eg: ls a b
    for(i=1; i<argc; i++)
        ls(argv[i]);
    exit(0);
}
