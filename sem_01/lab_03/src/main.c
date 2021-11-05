#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void)
{
    int childpid;
    if (-1 == (childpid = fork()))
    {
        perror("Can't fork\n");
        return EXIT_FAILURE;
    }
    else if (0 == childpid)
    {
        while (1)
            printf("%20d\n", getpid());
        return EXIT_SUCCESS;
    }
    else
    {
        while (1)
            printf("%10d\n", getpid());
        return 0;
    }
}