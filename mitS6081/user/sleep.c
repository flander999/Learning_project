#include "kernel/types.h"
#include "user/user.h"


int main(int args, char* argv[])
{
        if(args < 2){
                printf("Error: need insert a number as the lasting time...\n");
                exit(1);
        }

        int s_time = atoi(argv[1]);
        printf("Shell going to sleep %ds...\n", s_time);
 
        if(sleep(s_time) != 0){
                printf("sleep failed...\n");
                exit(1);
        };

//        sleep(s_time);
        exit(0);
}
