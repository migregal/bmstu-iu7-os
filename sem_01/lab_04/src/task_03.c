#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

#define ERR_OK 0
#define ERR_FORK 1
#define ERR_EXEC 2

#define CMD_CNT 2
#define CHILD_CNT 2
#define CHILD_SLP 2
#define BUFF_SZ 2048

int main()
{
  pid_t child_pids[CHILD_CNT] = {0};
  char *cmds[CMD_CNT] = {"./lab_05", "./lab_06"};
  char *args[CMD_CNT] = {"./", NULL};

  printf("parent born:PID = %d; PPID = %d; GROUP = %d\n",
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
      printf("child_%u born:PID = %d; PPID = %d; GROUP = %d\n",
             i, getpid(), getppid(), getpgrp());

      int cmd_i = i % CMD_CNT;

      int rc = execlp(cmds[cmd_i], cmds[cmd_i],
                      args[cmd_i], (char *)NULL);

      if (rc == -1)
      {
        fprintf(stderr, "exec failed\n");
        exit(ERR_EXEC);
      }

      assert(false);
    }
  }

  for (unsigned i = 0; i < CHILD_CNT; ++i)
  {
    int status, stat_val = 0;

    printf("parent waiting\n");
    pid_t childpid = wait(&status);
    printf("parent waited:child process (PID = %d) finished; "
           "status: %d\n",
           childpid, status);

    if (WIFEXITED(stat_val))
    {
      printf("parent talk:child process #%d "
             "finished with code: %d\n",
             i + 1, WEXITSTATUS(stat_val));
    }
    else if (WIFSIGNALED(stat_val))
    {
      printf("parent talk:child process #%d "
             "finished by signal with code: %d\n",
             i + 1, WTERMSIG(stat_val));
    }
    else if (WIFSTOPPED(stat_val))
    {
      printf("parent talk:child process #%d "
             "finished stopped with code: %d\n",
             i + 1, WSTOPSIG(stat_val));
    }
  }

  printf("parent died:PID = %d; PPID = %d; GROUP = %d\n",
         getpid(), getppid(), getpgrp());
}
