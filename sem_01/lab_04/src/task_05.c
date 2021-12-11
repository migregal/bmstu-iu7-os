#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#define ERR_OK 0
#define ERR_FORK 1
#define ERR_EXEC 2
#define ERR_PIPE 3

#define MSG_CNT 2
#define CHILD_CNT 2
#define CHILD_SLP 2
#define BUFF_SZ 2048

#define MOD_PRINT 0
#define MOD_QUIET 1

int mode = MOD_QUIET;

void sig_change_mod(int signum) { mode = MOD_PRINT; }

int main()
{
  signal(SIGINT, sig_change_mod);

  int fd[2];
  char buffer[BUFF_SZ] = {0};
  pid_t child_pids[CHILD_CNT] = {0};
  char *messages[MSG_CNT] = {
      "Maybe I maybe you\n",
      "The autumn leaves of red and gold\n"};

  if (pipe(fd) == -1)
  {
    fprintf(stderr, "Can't pipe\n");
    exit(ERR_PIPE);
  }

  printf("parent born:PID = %d; PPID = %d; GROUP = %d\n",
         getpid(), getppid(), getpgrp());

  for (unsigned i = 0; i < CHILD_CNT; ++i, sleep(2))
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

      if (mode == MOD_PRINT)
      {
        close(fd[0]);
        write(fd[1], messages[i], strlen(messages[i]));
        printf("child_%u send:PID = %d; MSG = %s\n",
               i, getpid(), messages[i]);
      }
      else
      {
        printf("child_%u send:quiet mode not message send\n", i);
      }

      exit(ERR_OK);
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

  close(fd[1]);
  ssize_t readed = read(fd[0], buffer, BUFF_SZ);

  if (readed == -1)
    printf("error on read\n");

  printf("parent recv:\n\n%s\n", buffer);
  printf("parent died:PID = %d; PPID = %d; GROUP = %d\n",
         getpid(), getppid(), getpgrp());
}
