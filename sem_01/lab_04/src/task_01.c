#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define ERR_OK 0
#define ERR_FORK 1

#define CHILD_CNT 2
#define CHILD_SLP 2

int main()
{
  pid_t child_pids[CHILD_CNT] = {0};

  printf("parent born: PID = %d; PPID = %d; GROUP = %d\n",
         getpid(), getppid(), getpgrp());

  for (unsigned i = 0; i < CHILD_CNT; ++i)
  {
    pid_t pid = fork();

    if (pid == -1)
    {
      fprintf(stderr, "Can't fork\n");
      exit(ERR_FORK);
    }

    if (pid)
    {
      child_pids[i] = pid;
    }
    else
    {
      printf("child_%u born: PID = %d; PPID = %d; GROUP = %d\n",
             i, getpid(), getppid(), getpgrp());

      sleep(CHILD_SLP);

      printf("child_%u died: PID = %d; PPID = %d; GROUP = %d\n",
             i, getpid(), getppid(), getpgrp());

      exit(ERR_OK);
    }
  }

  printf("parent died:PID = %d; PPID = %d; GROUP = %d\n",
         getpid(), getppid(), getpgrp());
}
