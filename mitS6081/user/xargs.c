#include "kernel/types.h"
#include "kernel/param.h"
#include "kernel/stat.h"
#include "user/user.h"

#define NBUFF 64

//为了保证原进程可以继续执行，需要向shell一样，使用子进程执行exec
void runcommand(char* command, char** arguments){
        if(fork() == 0){
                exec(command, arguments);
                exit(1);
        }
        else{
                wait(0);
        }
}

int main(int argc, char* argv[])
{
        // get the arguments
        if(argc < 3){
                fprintf(2, "need arguments to execute the command...\n");
                exit(1);
        }
        
        // 字符指针数组存储需要执行的命令字符串
        char* arguments[MAXARG];

        // 将需要传入的命令首先写入字符指针数组
        int i,j;
        for(i = 1, j = 0; i < argc; i++, j++){
                arguments[j] = argv[i];
        }

        //buf数组用于存储从标准输入读取的字符串
        char buf[NBUFF];
        memset(buf, 0, sizeof(buf));

        // read eacah lines from standard input as additional arguments
        // execute the command with these arguments
        while(1){
               gets(buf,sizeof(buf));
               if(buf[0] == 0){
                       break;
               }

               // chop \n
               buf[strlen(buf)-1] = 0;
               if(j > MAXARG){
                       printf("Too many arguments....\n");
               }

               // run the target cmd for each line from stdin
               arguments[j] = buf;
               runcommand(arguments[0], arguments);
        }

        exit(0);
}
