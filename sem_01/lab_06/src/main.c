#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include <time.h>

#include <windows.h>

#define READERS_COUNT 5
#define WRITERS_COUNT 3
#define ITERATIONS 10

#define MIN_SLEEP 300
#define MAX_RANDOM 4000

HANDLE writers[WRITERS_COUNT];
HANDLE readers[READERS_COUNT];

HANDLE mutex;
HANDLE can_read;
HANDLE can_write;

__volatile__ LONG waiting_writers = 0;
__volatile__ LONG waiting_readers = 0;
__volatile__ LONG active_readers = 0;

bool is_writer_active = false;

static __volatile__ int value = 0;

void strat_read(void) {
    InterlockedIncrement(&waiting_readers);

    if (is_writer_active || WaitForSingleObject(can_write, 0) == WAIT_OBJECT_0)
        WaitForSingleObject(can_read, INFINITE);

    WaitForSingleObject(mutex, INFINITE);

    InterlockedDecrement(&waiting_readers);
    InterlockedIncrement(&active_readers);

    SetEvent(can_read);
    ReleaseMutex(mutex);
}

void stop_read(void) {
    InterlockedDecrement(&active_readers);
    if (waiting_readers == 0) {
        SetEvent(can_write);
    }
}

DWORD WINAPI reader(LPVOID lpParams) {
    int r_id = (int)lpParams;
    srand(time(NULL) + r_id);

    while (value < WRITERS_COUNT * ITERATIONS) {
        int sleep_time = MIN_SLEEP + rand() % MAX_RANDOM;
        Sleep(sleep_time);

        strat_read();

        printf("=> Reader #%d read:  %3d -- idle %dms\n",
                                        (int)lpParams,
                                        value,
                                        sleep_time);
        stop_read();
    }

    return EXIT_SUCCESS;
}

void start_write(void) {
    InterlockedIncrement(&waiting_writers);

    if (is_writer_active || WaitForSingleObject(can_read, 0) == WAIT_OBJECT_0)
        WaitForSingleObject(can_write, INFINITE);

    InterlockedDecrement(&waiting_writers);

    is_writer_active = true;
    ResetEvent(can_write);
}

void stop_write(void) {
    is_writer_active = false;

    if (waiting_writers == 0) {
        SetEvent(can_read);
    } else {
        SetEvent(can_write);
    }
}

DWORD WINAPI writer(LPVOID lpParams) {
    int w_id = (int)lpParams;
    srand(time(NULL) + w_id);

    for (short i = 0; i < ITERATIONS; ++i) {
        int sleep_time = MIN_SLEEP + rand() % MAX_RANDOM;
        Sleep(sleep_time);

        start_write();

        ++value;
        printf("<= Writer #%d write: %3d -- idle %dms\n",
                                        (int)lpParams,
                                        value,
                                        sleep_time);
        stop_write();
    }

    return EXIT_SUCCESS;
}

int init(void) {
    if ((mutex = CreateMutex(NULL, FALSE, NULL)) == NULL) {
        perror("create mutex error!");
        return EXIT_FAILURE;
    }

    if ((can_read = CreateEvent(NULL, FALSE, TRUE, NULL)) == NULL) {
        perror("create event `can_read` error!");
        return EXIT_FAILURE;
    }
    if ((can_write = CreateEvent(NULL, TRUE, TRUE, NULL)) == NULL) {
        perror("create event `can_write` error!");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

int create_threads(HANDLE *threads,
                   int threads_count,
                   DWORD (*on_thread)(LPVOID)) {
    for (short i = 0; i < threads_count; ++i) {
        if ((threads[i] = CreateThread(NULL, 0, on_thread, (LPVOID)i, 0,
                                       NULL)) == NULL) {
            perror("create thread error!");
            return EXIT_FAILURE;
        }
    }

    return EXIT_SUCCESS;
}

int main(void) {
    setbuf(stdout, NULL);

    if (init() != EXIT_SUCCESS ||
            create_threads(readers, READERS_COUNT, reader) != EXIT_SUCCESS ||
            create_threads(writers, WRITERS_COUNT, writer) != EXIT_SUCCESS)
        return EXIT_FAILURE;

    WaitForMultipleObjects(WRITERS_COUNT, writers, TRUE, INFINITE);
    WaitForMultipleObjects(READERS_COUNT, readers, TRUE, INFINITE);

    CloseHandle(mutex);
    CloseHandle(can_read);
    CloseHandle(can_write);

    return EXIT_SUCCESS;
}
