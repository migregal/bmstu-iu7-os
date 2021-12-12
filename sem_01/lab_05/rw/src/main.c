#include <stdio.h>
#include <stdlib.h>

#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/stat.h>

#include <unistd.h>
#include <wait.h>

#include <monitor.h>

#include <reader.h>
#include <writer.h>

static inline void start_subprocess(
        int *shcntr,
        const int sem_id,
        const int count,
        int (*sub)(int *const, const int, const int)) {
    int chpid;
    for (size_t i = 0; i < count; ++i) {
        if ((chpid = fork()) == -1) {
            perror("fork failed");
            exit(1);
        }
        if (chpid == 0) {
            sub(shcntr, sem_id, i);
            exit(0);
        }
    }
}

int main() {
    setbuf(stdout, NULL);

    int perms = S_IRWXU | S_IRWXG | S_IRWXO;
    int fd = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | perms);
    if (fd == -1) {
        perror("shmget failed");
        return EXIT_FAILURE;
    }

    int *shcntr = shmat(fd, 0, 0);
    if (shcntr == (void *) -1) {
        perror("shmat failed");
        return EXIT_FAILURE;
    }

    int sem_id = semget(IPC_PRIVATE, MAX_SEMS, IPC_CREAT | perms);
    if (sem_id == -1) {
        perror("semget failed");
        return EXIT_FAILURE;
    }

    semctl(sem_id, ACTIVE_READERS, SETVAL, 0);
    semctl(sem_id, ACTIVE_WRITERS, SETVAL, 0);
    semctl(sem_id, WAITING_WRITERS, SETVAL, 0);
    semctl(sem_id, WAITING_READERS, SETVAL, 0);

    start_subprocess(shcntr, sem_id, READERS_COUNT, r_run);
    start_subprocess(shcntr, sem_id, WRITERS_COUNT, w_run);

    for (size_t i = 0; i < WRITERS_COUNT + READERS_COUNT; ++i) {
        int status;
        if (wait(&status) == -1) {
            perror("child error");
            exit(1);
        }
        if (!WIFEXITED(status)) {
            printf("child process terminated abnormally\n");
        }
    }

    if (shmdt((void *) shcntr) == -1 ||
        shmctl(fd, IPC_RMID, NULL) == -1 ||
        semctl(sem_id, IPC_RMID, 0) == -1) {

        perror("exit error");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
