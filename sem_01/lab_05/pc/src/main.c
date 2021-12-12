#include <stdio.h>

#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/stat.h>

#include <unistd.h>
#include <wait.h>

#include <buf.h>

#include <consumer.h>
#include <producer.h>

#define MAX_SEMS 3

static inline void start_subprocess(
        cbuf_t *buf,
        const int sem_id,
        const int count,
        int (*sub)(cbuf_t *const, const int, const int)) {
    int chpid;
    for (size_t i = 0; i < count; ++i) {
        if ((chpid = fork()) == -1) {
            perror("fork failed");
            exit(1);
        }
        if (chpid == 0) {
            sub(buf, sem_id, i);
            exit(0);
        }
    }
}

int main() {
    setbuf(stdout, NULL);

    int perms = S_IRWXU | S_IRWXG | S_IRWXO;
    int fd = shmget(IPC_PRIVATE, sizeof(cbuf_t), IPC_CREAT | perms);
    if (fd == -1) {
        perror("shmget failed");
        return EXIT_FAILURE;
    }

    cbuf_t *buf = shmat(fd, 0, 0);
    if (buf == (void *) -1) {
        perror("shmat failed");
        return EXIT_FAILURE;
    }

    if (init_buf(buf) == -1) {
        perror("buffer initialization failed");
        return EXIT_FAILURE;
    }

    int sem_id = semget(IPC_PRIVATE, MAX_SEMS, IPC_CREAT | perms);
    if (sem_id == -1) {
        perror("semget failed");
        return EXIT_FAILURE;
    }

    semctl(sem_id, BIN_SEM, SETVAL, 1);
    semctl(sem_id, BUF_EMPTY, SETVAL, N);
    semctl(sem_id, BUF_FULL, SETVAL, 0);

    start_subprocess(buf, sem_id, P_COUNT, p_run);
    start_subprocess(buf, sem_id, C_COUNT, c_run);

    for (size_t i = 0; i < C_COUNT + P_COUNT; ++i) {
        int status;
        if (wait(&status) == -1) {
            perror("child error");
            exit(1);
        }
        if (!WIFEXITED(status)) {
            printf("child process terminated abnormally\n");
        }
    }

    if (shmdt((void *) buf) == -1 ||
        shmctl(fd, IPC_RMID, NULL) == -1 ||
        semctl(sem_id, IPC_RMID, 0) == -1) {
        perror("exit error.");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
