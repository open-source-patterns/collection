/**
 * @file test_mutex.c
 * @brief Mutex Unit Test
 *
 * @author Saad Shams https://linkedin.com/in/muizz
 * @copyright BSD 3-Clause License
 */
#include "test_mutex.h"
#include "collection/i_platform.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

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

void test_mutex_basic(void) { // Mutex Basic Contention Test
    pthread_t threads[THREAD_COUNT];

    mutex_init(&counter_mutex);
    shared_counter = 0;

    for (int i = 0; i < THREAD_COUNT; ++i)
        assert(pthread_create(&threads[i], NULL, counter_thread, NULL) == 0);

    for (int i = 0; i < THREAD_COUNT; ++i)
        assert(pthread_join(threads[i], NULL) == 0);

    mutex_destroy(&counter_mutex);

    assert(shared_counter == THREAD_COUNT * ITERATIONS);
}

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

void test_mutex_once(void) { // Mutex Once Test
    pthread_t threads[THREAD_COUNT];

    once_counter = 0;

    for (int i = 0; i < THREAD_COUNT; ++i)
        assert(pthread_create(&threads[i], NULL, once_thread, NULL) == 0);

    for (int i = 0; i < THREAD_COUNT; ++i)
        assert(pthread_join(threads[i], NULL) == 0);

    assert(once_counter == 1);
}

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

void test_mutex_once_with_mutex(void) { // Mutex Once + Mutex Interaction
    pthread_t threads[THREAD_COUNT];

    once_token = (MutexOnce)MUTEX_ONCE_INIT;
    init_value = 0;

    for (int i = 0; i < THREAD_COUNT; ++i)
        assert(pthread_create(&threads[i], NULL, complex_thread, NULL) == 0);

    for (int i = 0; i < THREAD_COUNT; ++i)
        assert(pthread_join(threads[i], NULL) == 0);

    mutex_destroy(&init_mutex);
}
