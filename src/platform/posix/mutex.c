#include "collection/i_platform.h"

#include <pthread.h>
#include <stdlib.h>

#pragma region Synchronization

// Initializes a mutex.
int mutex_init(Mutex *mutex) {
    if (mutex == NULL) return -1;
    return pthread_rwlock_init(&mutex->rwlock, NULL);
}

// Acquires an exclusive mutex lock.
int mutex_lock(Mutex *mutex) {
    if (mutex == NULL) return -1;
    return pthread_rwlock_wrlock(&mutex->rwlock);
}

// Acquires a shared mutex lock.
int mutex_lock_shared(Mutex *mutex) {
    if (mutex == NULL) return -1;
    return pthread_rwlock_rdlock(&mutex->rwlock);
}

// Releases a mutex lock.
int mutex_unlock(Mutex *mutex) {
    if (mutex == NULL) return -1;
    return pthread_rwlock_unlock(&mutex->rwlock);
}

// Destroys a mutex.
int mutex_destroy(Mutex *mutex) {
    if (mutex == NULL) return -1;
    return pthread_rwlock_destroy(&mutex->rwlock);
}

#pragma endregion

#pragma region One-Time Initialization

// Executes a callback exactly once.
int mutex_once(MutexOnce *once, void (*callback)(void)) {
    if (once == NULL || callback == NULL) return -1;
    return pthread_once(once, callback);
}

#pragma endregion