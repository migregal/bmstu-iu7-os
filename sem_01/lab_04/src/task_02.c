#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

#define ERR_OK 0
#define ERR_FORK 1

#define CHILD_CNT 2
#define CHILD_SLP 2

int main()
{
  pid_t child_pids[CHILD_CNT] = {0};

  printf("parent born:PID = %d; PPID = %d; GROUP = %d\n",
         getpid(), getppid(), getpgrp());

  for (unsigned i = 0; i < CHILD_CNT; ++i)
  {
    pid_t pid = fork();

    if (-1 == pid)
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
      // child
      printf("child%u born:PID = %d; PPID = %d; GROUP = %d\n",
             i, getpid(), getppid(), getpgrp());

      sleep(CHILD_SLP);

      printf("child%u died:PID = %d; PPID = %d; GROUP = %d\n",
             i, getpid(), getppid(), getpgrp());

      exit(ERR_OK);
    }
  }

  for (unsigned i = 0; i < CHILD_CNT; ++i)
  {
    int status, stat_val = 0;

    printf("parent waiting\n");
    pid_t childpid = wait(&status);
    printf("parent waited:child process (PID = %d) finished. status: %d\n",
           childpid, status);

    if (WIFEXITED(stat_val))
    {
      printf("parent talk:child process #%d finished with code: %d\n", i + 1,
             WEXITSTATUS(stat_val));
    }
    else if (WIFSIGNALED(stat_val))
    {
      printf("parent talk:child process #%d finished by signal with code: %d\n",
             i + 1, WTERMSIG(stat_val));
    }
    else if (WIFSTOPPED(stat_val))
    {
      printf("parent talk:child process #%d stopped with code: %d\n",
             i + 1, WSTOPSIG(stat_val));
    }
  }

  printf("parent died:PID = %d; PPID = %d; GROUP = %d\n",
         getpid(), getppid(), getpgrp());
}
