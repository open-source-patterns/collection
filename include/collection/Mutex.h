/**
* @file mutex.h
 * @brief Cross-platform mutex abstraction for thread synchronization.
 *
 * Provides a unified interface for mutex operations on Windows and POSIX systems.
 * On Windows, it uses CRITICAL_SECTION for exclusive locking.
 * On POSIX systems, it uses pthread read-write locks with support for shared (read) and exclusive (write) locking.
 *
 * Macros:
 * - mutex_init(m): Initialize the mutex.
 * - mutex_destroy(m): Destroy the mutex and release resources.
 * - mutex_lock(m): Acquire exclusive (write) lock.
 * - mutex_unlock(m): Release the lock.
 * - mutex_lock_shared(m): Acquire shared (read) lock (equivalent to exclusive lock on Windows).
 *
 * Usage:
 * ```c
 * Mutex m;
 * mutex_init(&m);
 * mutex_lock(&m);
 * // critical section
 * mutex_unlock(&m);
 * mutex_destroy(&m);
 * ```
 */

#pragma once

#ifdef _WIN32
#include <windows.h>
typedef struct { CRITICAL_SECTION cs; } Mutex;
#define mutex_init(m) InitializeCriticalSection(&(m)->cs)
#define mutex_destroy(m) DeleteCriticalSection(&(m)->cs)
#define mutex_lock(m) EnterCriticalSection(&(m)->cs)
#define mutex_unlock(m) LeaveCriticalSection(&(m)->cs)
#define mutex_lock_shared(m) mutex_lock(m) // Windows doesn't support rwlock natively, so read/write distinction is the same

typedef INIT_ONCE MutexOnce;
#define MUTEX_ONCE_INIT INIT_ONCE_STATIC_INIT

static BOOL CALLBACK dispatchOnceWin(PINIT_ONCE InitOnce, PVOID Parameter, PVOID *Context) {
 (void)InitOnce; (void)Context; (void)Parameter;
 mutex_init((Mutex *)Parameter);  // Initialize your mutex here
 return TRUE;
}

#define mutex_once(mutexOncePtr, fn) \
InitOnceExecuteOnce(mutexOncePtr, dispatchOnceWin, &(mutex), NULL)
#else
#include <pthread.h>
typedef pthread_rwlock_t Mutex;
#define mutex_init(m) pthread_rwlock_init(m, NULL)
#define mutex_destroy(m) pthread_rwlock_destroy(m)
#define mutex_lock(m) pthread_rwlock_wrlock(m)
#define mutex_unlock(m) pthread_rwlock_unlock(m)
#define mutex_lock_shared(m) pthread_rwlock_rdlock(m)

typedef pthread_once_t MutexOnce;
#define MUTEX_ONCE_INIT PTHREAD_ONCE_INIT
#define mutex_once(m, fn) pthread_once(m, fn)
#endif
