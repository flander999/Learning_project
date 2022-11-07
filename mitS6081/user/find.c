#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

char* fmtname(char* path)
{
        static char buf[DIRSIZ+1];
        char* p;

        for(p = path+strlen(path); p >= path && *p != '/'; p--)
            ; 
        p++;

        if(strlen(p) > DIRSIZ)
                return p;
        memmove(buf, p, strlen(p));
        memset(buf+strlen(p), ' ', DIRSIZ-strlen(p));
        return buf;
}


void find(char* path, char* filename)
{
        char buf[512], *p;
        int fd;
        struct dirent de;
        struct stat st;

        if((fd = open(path,0)) < 0){
                fprintf(2,"find: cannot open %s\n", path);
                return;
        }

        if(fstat(fd, &st) < 0){
                fprintf(2, "find: cannot stat %s\n", path);
                close(fd);
                return;
        }

        while(read(fd, &de, sizeof(de)) == sizeof(de)){
                strcpy(buf, path);
                p = buf+strlen(buf);
                *p++ = '/';

                if(de.inum == 0)
                        continue;

                memmove(p, de.name, DIRSIZ);
                p[DIRSIZ] = 0;

                if(stat(buf, &st) < 0){
                        fprintf(2, "find cannot open %s...\n", buf);
                }

                switch(st.type){
                        case T_DEVICE:
                        case T_FILE:
                                if(strcmp(de.name, filename) == 0){
                                        printf("%s\n", buf);
                                }
                                break;
                        case T_DIR:
                                if(strcmp(de.name, ".") != 0 && strcmp(de.name, "..") != 0){
                                        find(buf,filename);
                                }
                }

        }
        close(fd);
        //return;
        
}

int main(int argc, char* argv[])
{
        if(argc < 2){
                printf("Need set target filename\n");
                exit(1);
        }
        
        // not set the root dir, try to start at current dir
        if(argc == 2){
                find(".",argv[1]);
                exit(0);
        }
        else if(argc == 3){
                find(argv[1],argv[2]);
                exit(0);
        }
        else{
                fprintf(2,"To many parameters...\n");
                exit(1);
        }
}
