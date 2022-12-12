#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void sieve(int);


int main(int args, char* argv[]){
        if(args > 1){
                printf("Too many parameters...\n");
                exit(0);
        }


        int p[2];
        pipe(p);

        // main process is going to send numbers from 2 to 35. It is not responsible for
        // filtering. And it needs to send -1 as a signal to end all these process.
        if(fork() != 0){
                close(p[0]); // since main proc do not need read from any others
                for(int i = 2; i < 35; i++){
                        write(p[1],&i,sizeof(int));
                }
                int buf = -1;
                write(p[1], &buf, sizeof(int));
                wait(0);
                exit(0);
        }
        else{
                close(p[1]); // since child proc do not need to write back to main proc
                // call sieve() to start a filter
                sieve(p[0]);
                //close(p[0]);
        }
        exit(0);
}


/*
 * the sieve is a filter. It accept an read file descriptor to get the printable number.
 * Then if this is not the end, it start to create another process and pass the numbers
 * that read from left process to right process. Any numbers that can be divided by n will
 * be droped.
 */
void sieve(int fd){
        int n;
        // If cannot read from left process, then stop the process and send error message
        if(read(fd,&n,sizeof(int)) == -1){
                printf("Channel does not work...\n");
                exit(1);
        }

        // -1 is a signal that this is the end, process should immediately end.
        if(n == -1){
                exit(0);
        }
        printf("prime %d\n", n);

        int p[2];
        pipe(p);

        // Child process call sieve() to start another filter to filter other numbers.
        // Since it does not need fd to read and it also does not need to write to its
        // left process, therefore it also close p[1];
        if(fork() == 0){
                close(fd);
                close(p[1]);
                sieve(p[0]);
        }
        // Process run as a filter to drop non-primes. Since it does not need to read from
        // left process, p[0] is closed. After the while loop, it means buf get value -1
        // which means it is the end. And before exit, it needs to wait its child process
        // finish firstly.
        else{
               close(p[0]);
               int buf;
               while(read(fd, &buf, sizeof(int)) && buf != -1){
                       if(buf%n != 0){
                               write(p[1],&buf,sizeof(int));
                       }
               }
               buf = -1;
               write(p[1],&buf,sizeof(int));
               wait(0);
        }

        exit(0);
}
