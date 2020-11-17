#include <sys/types.h> 
#include <stdio.h> 
#include <unistd.h>

// this is unix os orriented code not be able to compiled in windows
int main() 
{
    pid_t pid = fork();
    if (pid == 0) {
        execlp("/bin/echo","echo", "Hello from the child process",NULL); 
        printf("Bye"); 
    } else if(pid > 0) { 
        wait(NULL);
        printf("Hello from the parent process\n");
    }
        printf("Bye\n");
    return 0;
}
