#include <monitor.h>

#include <sys/sem.h>

static struct sembuf START_READ[] = {
        {WAITING_READERS, 1, 0},
        {ACTIVE_WRITERS, 0, 0},
        {WAITING_WRITERS, 0, 0},
        {ACTIVE_READERS, 1, 0},
        {WAITING_READERS, -1, 0},
};

static struct sembuf STOP_READ[] = {
        {ACTIVE_READERS, -1, 0},
};

static struct sembuf START_WRITE[] = {
        {WAITING_WRITERS, 1, 0},
        {ACTIVE_READERS, 0, 0},
        {ACTIVE_WRITERS, 0, 0},
        {ACTIVE_WRITERS, 1, 0},
        {WAITING_WRITERS, -1, 0},
};

static struct sembuf STOP_WRITE[] = {
        {ACTIVE_WRITERS, -1, 0},
};

int start_read(int sem_id) {
    return semop(sem_id, START_READ, 5) == -1;
}

int stop_read(int sem_id) {
    return semop(sem_id, STOP_READ, 1) == -1;
}

int start_write(int sem_id) {
    return semop(sem_id, START_WRITE, 5) == -1;
}

int stop_write(int sem_id) {
    return semop(sem_id, STOP_WRITE, 1) == -1;
}
