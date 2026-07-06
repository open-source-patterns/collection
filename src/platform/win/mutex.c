#include "collection/i_platform.h"

#include <stdlib.h>

#pragma region Synchronization

// Initializes a mutex.
int mutex_init(Mutex *mutex) {
    if (mutex == NULL) return -1;
    InitializeCriticalSection(&mutex->cs);
    return 0;
}

// Acquires an exclusive mutex lock.
int mutex_lock(Mutex *mutex) {
    if (mutex == NULL) return -1;
    EnterCriticalSection(&mutex->cs);
    return 0;
}

// Acquires a shared mutex lock.
int mutex_lock_shared(Mutex *mutex) {
    if (mutex == NULL) return -1;
    EnterCriticalSection(&mutex->cs);
    return 0;
}

// Releases a mutex lock.
int mutex_unlock(Mutex *mutex) {
    if (mutex == NULL) return -1;
    LeaveCriticalSection(&mutex->cs);
    return 0;
}

// Destroys a mutex.
int mutex_destroy(Mutex *mutex) {
    if (mutex == NULL) return -1;
    DeleteCriticalSection(&mutex->cs);
    return 0;
}

// Adapts the Windows one-time initialization callback.
static BOOL CALLBACK mutex_once_callback(PINIT_ONCE InitOnce, PVOID Parameter, PVOID* Context) {
    (void) InitOnce;
    (void) Context;

    void (*init)(void) = (void (*)(void)) Parameter;
    init();

    return TRUE;
}

#pragma endregion

#pragma region One-Time Initialization

// Executes a callback exactly once.
int mutex_once(MutexOnce *once, void (*callback)(void)) {
    InitOnceExecuteOnce(once, mutex_once_callback, (PVOID)callback, NULL);
    return 0;
}

#pragma endregion
