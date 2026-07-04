/**
 * @file test_mutex.c
 * @brief Mutex unit tests.
 *
 * @author Saad Shams https://linkedin.com/in/muizz
 * @copyright BSD 3-Clause License
 */
#include "test_mutex.h"
#include "collection/i_platform.h"

#include <stdio.h>
#include <stdlib.h>

#ifdef _WIN32
typedef HANDLE Thread;
typedef DWORD WINAPI ThreadResult;
#define THREAD_RETURN 0
#else
typedef pthread_t Thread;
typedef void *ThreadResult;
#define THREAD_RETURN NULL
#endif

#define THREAD_COUNT 16
#define ITERATIONS   10000

static void before_all(void) {}
static void before_each(void) {}
static void after_each(void) {}
static void after_all(void) {}

static void test(const char *name, void (*callback)(void)) {
    printf("\033[0;34m[RUNNING]\033[0m %s...\n", name);
    fflush(stdout);

    before_each();
    callback();
    after_each();

    printf("\033[0;32m[PASSED]\033[0m %s\n", name);
    fflush(stdout);
}

int main(void) {
    printf("\n\033[1;36m================================================\033[0m\n");
    printf("\033[1;36m[SUITE] %s\033[0m\n", "MutexTest");
    printf("\033[1;36m================================================\033[0m\n\n");

    before_all();
    test("test_mutex_basic", test_mutex_basic);
    test("test_mutex_once", test_mutex_once);
    test("test_mutex_once_with_mutex", test_mutex_once_with_mutex);
    after_all();

    printf("\n\033[1;32m[DONE] All tests in suite finished.\033[0m\n");
    return 0;
}

// Creates a platform thread.
static void thread_create(Thread *thread, ThreadResult (*routine)(void *), void *arg) {
#ifdef _WIN32
    *thread = CreateThread(NULL, 0, routine, arg, 0, NULL);
    if (*thread == NULL) abort();
#else
    if (pthread_create(thread, NULL, routine, arg) != 0) abort();
#endif
}

// Waits for a platform thread to finish.
static void thread_join(Thread thread) {
#ifdef _WIN32
    if (WaitForSingleObject(thread, INFINITE) != WAIT_OBJECT_0) abort();
    CloseHandle(thread);
#else
    if (pthread_join(thread, NULL) != 0) abort();
#endif
}

static Mutex counter_mutex;
static int shared_counter = 0;

// Increments the shared counter under mutex protection.
static ThreadResult counter_thread(void *arg) {
    (void) arg;
    for (int i = 0; i < ITERATIONS; i++) {
        if (mutex_lock(&counter_mutex) != 0) abort();
        shared_counter++;
        if (mutex_unlock(&counter_mutex) != 0) abort();
    }
    return THREAD_RETURN;
}

// Verifies mutex locking under concurrent contention.
void test_mutex_basic(void) {
    Thread threads[THREAD_COUNT];

    if (mutex_init(&counter_mutex) != 0) abort();
    shared_counter = 0;

    for (int i = 0; i < THREAD_COUNT; ++i)
        thread_create(&threads[i], counter_thread, NULL);

    for (int i = 0; i < THREAD_COUNT; ++i)
        thread_join(threads[i]);

    if (mutex_destroy(&counter_mutex) != 0) abort();
    if (shared_counter != THREAD_COUNT * ITERATIONS) abort();
}

static MutexOnce once_token_basic = MUTEX_ONCE_INIT;
static int once_counter = 0;

// Initializes once-only state.
static void once_init(void) {
    once_counter++;
}

// Invokes once-only initialization from a thread.
static ThreadResult once_thread(void *arg) {
    (void) arg;
    if (mutex_once(&once_token_basic, once_init) != 0) abort();
    return THREAD_RETURN;
}

// Verifies once-only initialization across multiple threads.
void test_mutex_once(void) {
    Thread threads[THREAD_COUNT];
    once_counter = 0;

    for (int i = 0; i < THREAD_COUNT; ++i)
        thread_create(&threads[i], once_thread, NULL);

    for (int i = 0; i < THREAD_COUNT; ++i)
        thread_join(threads[i]);

    if (once_counter != 1) abort();
}

static MutexOnce once_token_with_mutex = MUTEX_ONCE_INIT;
static Mutex init_mutex;
static int init_value = 0;

// Initializes mutex-protected shared state once.
static void complex_init(void) {
    if (mutex_init(&init_mutex) != 0) abort();
    init_value = 42;
}

// Verifies once-initialized mutex-protected state from a thread.
static ThreadResult complex_thread(void *arg) {
    (void) arg;

    if (mutex_once(&once_token_with_mutex, complex_init) != 0) abort();

    if (mutex_lock(&init_mutex) != 0) abort();
    if (init_value != 42) abort();
    if (mutex_unlock(&init_mutex) != 0) abort();

    return THREAD_RETURN;
}

// Verifies one-time initialization of mutex-protected shared state.
void test_mutex_once_with_mutex(void) {
    Thread threads[THREAD_COUNT];
    init_value = 0;

    for (int i = 0; i < THREAD_COUNT; ++i)
        thread_create(&threads[i], complex_thread, NULL);

    for (int i = 0; i < THREAD_COUNT; ++i)
        thread_join(threads[i]);

    if (mutex_destroy(&init_mutex) != 0) abort();
}
