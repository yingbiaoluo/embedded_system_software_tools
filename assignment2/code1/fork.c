#include <unistd.h>
#include <stdio.h>

int main()
{
    int pid = fork();

    if (pid == -1)
        return -1;

    if (pid)
    {
        printf("I am father, my pid is %d\n", getpid());
        return 0;
    }
    else
    {
        printf("I am child, my pid is %d\n", getpid());
        return 0;
    }
}
