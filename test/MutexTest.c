#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "collection/Mutex.h"
#include "MutexTest.h"

#define THREAD_COUNT 16
#define ITERATIONS   100000

int main(void) {
    printf("Running mutex unit tests...\n");

    test_mutex_basic();
    test_mutex_once();
    test_mutex_once_with_mutex();

    printf("All tests passed ✅\n");
    return 0;
}

/* -------------------------------------------------------
 * Test 1: mutex correctness under contention
 * ------------------------------------------------------- */

static Mutex counter_mutex;
static int shared_counter = 0;

void* counter_thread(void* arg) {
    (void)arg;
    for (int i = 0; i < ITERATIONS; ++i) {
        mutex_lock(&counter_mutex);
        shared_counter++;
        mutex_unlock(&counter_mutex);
    }
    return NULL;
}

void test_mutex_basic(void) {
    pthread_t threads[THREAD_COUNT];

    mutex_init(&counter_mutex);
    shared_counter = 0;

    for (int i = 0; i < THREAD_COUNT; ++i)
        assert(pthread_create(&threads[i], NULL, counter_thread, NULL) == 0);

    for (int i = 0; i < THREAD_COUNT; ++i)
        assert(pthread_join(threads[i], NULL) == 0);

    mutex_destroy(&counter_mutex);

    assert(shared_counter == THREAD_COUNT * ITERATIONS);
    printf("[OK] mutex basic contention test\n");
}

/* -------------------------------------------------------
 * Test 2: mutex_once correctness
 * ------------------------------------------------------- */

static MutexOnce once_token = MUTEX_ONCE_INIT;
static int once_counter = 0;

static void once_init(void) {
    once_counter++;
}

static void* once_thread(void* arg) {
    (void)arg;
    mutex_once(&once_token, once_init);
    return NULL;
}

void test_mutex_once(void) {
    pthread_t threads[THREAD_COUNT];

    once_counter = 0;

    for (int i = 0; i < THREAD_COUNT; ++i)
        assert(pthread_create(&threads[i], NULL, once_thread, NULL) == 0);

    for (int i = 0; i < THREAD_COUNT; ++i)
        assert(pthread_join(threads[i], NULL) == 0);

    assert(once_counter == 1);
    printf("[OK] mutex_once executed exactly once\n");
}

/* -------------------------------------------------------
 * Test 3: mutex_once + mutex interaction
 * ------------------------------------------------------- */

static Mutex init_mutex;
static int init_value = 0;

static void complex_init(void) {
    mutex_init(&init_mutex);
    init_value = 42;
}

static void* complex_thread(void* arg) {
    (void)arg;

    mutex_once(&once_token, complex_init);

    mutex_lock(&init_mutex);
    assert(init_value == 42);
    mutex_unlock(&init_mutex);

    return NULL;
}

void test_mutex_once_with_mutex(void) {
    pthread_t threads[THREAD_COUNT];

    once_token = (MutexOnce)MUTEX_ONCE_INIT;
    init_value = 0;

    for (int i = 0; i < THREAD_COUNT; ++i)
        assert(pthread_create(&threads[i], NULL, complex_thread, NULL) == 0);

    for (int i = 0; i < THREAD_COUNT; ++i)
        assert(pthread_join(threads[i], NULL) == 0);

    mutex_destroy(&init_mutex);
    printf("[OK] mutex_once + mutex interaction test\n");
}
