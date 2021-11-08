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

int main()
{
  int fd[2];
  char buffer[BUFF_SZ] = {0};
  pid_t child_pids[CHILD_CNT] = {0};
  char *messages[MSG_CNT] = {"First\n", "Second\n"};

  if (-1 == pipe(fd))
  {
    fprintf(stderr, "Can't pipe\n");
    exit(ERR_PIPE);
  }

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

    if (0 != pid)
    {
      child_pids[i] = pid;
    }
    else
    {
      printf("child_%u born:PID = %d; PPID = %d; GROUP = %d\n",
             i, getpid(), getppid(), getpgrp());

      int msg_i = i % MSG_CNT;
      close(fd[0]);
      write(fd[1], messages[msg_i], strlen(messages[msg_i]));
      printf("child_%u send:PID = %d; MSG = %s\n",
             i, getpid(), messages[msg_i]);

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

  if (-1 == readed)
  {
    printf("error on read\n");
  }

  printf("parent recv:\n%s\n", buffer);
  printf("parent died:PID = %d; PPID = %d; GROUP = %d\n",
         getpid(), getppid(), getpgrp());
}
