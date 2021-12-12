#include <producer.h>

#include <stdio.h>

#include <sys/sem.h>

#include <time.h>
#include <unistd.h>

struct sembuf P_LOCK[2] = {{BUF_EMPTY, -1, 0}, {BIN_SEM, -1, 0}};
struct sembuf P_RELEASE[2] = {{BUF_FULL, 1, 0}, {BIN_SEM, 1, 0}};

int p_run(cbuf_t *const buf, const int sem_id, const int p_id) {
    if (!buf)
        return -1;

    srand(time(NULL) + p_id);

    int stime;
    char ch;

    for (size_t i = 0; i < ITERS; ++i) {
        stime = rand() % MAX_RAND_P + 1;
        sleep(stime);

        if (semop(sem_id, P_LOCK, 2) == -1) {
            perror("producer lock error");
            exit(1);
        }

        ch = 'a' + (char) (buf->w_pos % 26);

        if (write_buf(buf, ch) == -1) {
            perror("buffer write error");
            return EXIT_FAILURE;
        }

        printf("<= Producer #%d wrote: %c // Idle time: %ds\n", p_id, ch, stime);

        if (semop(sem_id, P_RELEASE, 2) == -1) {
            perror("producer release error");
            exit(1);
        }
    }

    return EXIT_SUCCESS;
}
