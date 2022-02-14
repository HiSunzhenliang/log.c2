#include <pthread.h>
#include "demo.h"

#define THREAD_NUM 2

pthread_mutex_t MUTEX_LOG;

static void log_lock(bool lock, void *udata) {
    pthread_mutex_t *LOCK = (pthread_mutex_t *)(udata);
    if (lock)
        pthread_mutex_lock(LOCK);
    else
        pthread_mutex_unlock(LOCK);
}

static void *demo_lock_thread(void *udata) {
    log_stdout_verbose(LOG_DEBUG);
    log_debug("this is a log. Thread: %d", *(int*)udata);
    return NULL;
}

int demo_lock_main() {
    pthread_mutex_init(&MUTEX_LOG, NULL);
    log_set_lock(log_lock, &MUTEX_LOG);

    pthread_t tid[THREAD_NUM];
    for (int i = 0; i < THREAD_NUM; i++) {
        int arg = i;
        pthread_create(&tid[i], NULL, demo_lock_thread, &arg);
        if (tid[i] == 0) {
            log_error("create thread failed.");
            return -1;
        }
        printf("create thread %d success.\n", i);
    }

    for(int i = 0; i < THREAD_NUM; i++) {
        pthread_join(tid[i], NULL);
        printf("join thread %d success.\n", i);
    }

    pthread_mutex_destroy(&MUTEX_LOG);
    return 0;
}
