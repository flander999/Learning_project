#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int args, char* argv[])
{
        if(args > 1){
                fprintf(2,"too many parameters...\n");
                exit(1);
        }
        
        // prepare two pipe for convinient closing unused pipe in different process
        int pp[2],pc[2]; // store pipe file descriptor, p[0] for read && p[1] for write
        pipe(pp); // pipe for write to child and read from parent
        pipe(pc); // pipe for write to parent and read from child

        if(fork() == 0){
                close(pp[1]);
                close(pc[0]);
                char buf;
                if(read(pp[0], &buf, 1) == -1){
                        printf("Child cannot read from parent...\n");
                        exit(1);
                } 

                printf("%d: received ping\n",getpid());

                if(write(pc[1],"c", 1) == -1){
                        printf("Child Cannot send out the message");
                        exit(1);
                }
                close(pp[0]);
                close(pc[1]);
        }
        else{
                close(pp[0]);
                close(pc[1]);
                char buf;
                if(write(pp[1],"p", 1) == -1){
                        printf("Parent cannot send out the message...\n");
                        exit(1);
                }
                wait(0);

                if(read(pc[0], &buf, 1) == -1){
                        printf("Parent cannot received message from child...\n");
                        exit(1);
                }

                printf("%d: received pong\n", getpid());

                close(pp[1]);
                close(pc[0]);
        }

        exit(0);

}
