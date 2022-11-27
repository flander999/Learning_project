#include "kernel/param.h"
#include "kernel/types.h"
#include "user/user.h"


int main(int argc, char* argv[])
{
        if(argc < 3){
                fprintf(2,"trace nned a target to traceback\n");
        }

        int trace_mask = atoi(argv[1]);
        char* arguments[MAXARG];

        // take out the shell command that really needs to exec
        for(int i = 2, j = 0; j < argc; i++, j++){
               arguments[j] = argv[i];
        }

        if(trace(trace_mask) < 0){
                printf("Usage: Flailed to trace system calls...\n");
                exit(1);
        }


        // start a new process to exec the real command and send the trace argument
        exec(arguments[0], arguments);
        exit(0);
}
