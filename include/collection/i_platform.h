/**
 * @file i_platform.h
 * @ingroup Collection
 * @brief Cross-platform mutex, once, alignment, and process statistics utilities.
 */
#pragma once

#pragma region Platform Types

#ifdef _WIN32
#ifndef WIN32_LEAN_AND_MEAN
    #define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#include <malloc.h>
#define alloca _alloca /* Maps alloca to MSVC's _alloca. */

/**
 * @brief Cross-platform mutex type.
 *
 * On Windows, this is backed by CRITICAL_SECTION.
 */
typedef struct Mutex {
    CRITICAL_SECTION cs; /**< Native Windows critical section. */
} Mutex;

/**
 * @brief Cross-platform one-time initialization token.
 *
 * On Windows, this is backed by INIT_ONCE.
 */
typedef INIT_ONCE MutexOnce;

/**
 * @brief Static initializer for MutexOnce.
 */
#define MUTEX_ONCE_INIT INIT_ONCE_STATIC_INIT

/* Alignment compatibility shim. */
#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L
    #include <stdalign.h>
#else
/* Fallback for pre-C11 or specialized compilers */
    #ifndef alignas
        #if defined(_MSC_VER)
            #define alignas(x) __declspec(align(x))
        #elif defined(__GNUC__) || defined(__clang__)
            #define alignas(x) __attribute__((aligned(x)))
        #else
            #define alignas(x)
        #endif
    #endif
#endif

#else
#include <pthread.h>
#include <alloca.h>
#include <stdalign.h>

/**
 * @brief Cross-platform mutex type.
 *
 * On POSIX platforms, this is backed by pthread_rwlock_t.
 */
typedef struct Mutex {
    pthread_rwlock_t rwlock; /**< Native POSIX read/write lock. */
} Mutex;

/**
 * @brief Cross-platform one-time initialization token.
 *
 * On POSIX platforms, this is backed by pthread_once_t.
 */
typedef pthread_once_t MutexOnce;

/**
 * @brief Static initializer for MutexOnce.
 */
#define MUTEX_ONCE_INIT PTHREAD_ONCE_INIT
#endif

#pragma endregion

#pragma region Synchronization

/**
 * @brief Initializes a mutex.
 *
 * @param mutex Pointer to the mutex to initialize.
 * @return 0 on success; non-zero on failure.
 */
int mutex_init(Mutex *mutex);

/**
 * @brief Acquires an exclusive mutex lock.
 *
 * @param mutex Pointer to the mutex.
 * @return 0 on success; non-zero on failure.
 */
int mutex_lock(Mutex *mutex);

/**
 * @brief Acquires a shared/read mutex lock.
 *
 * @param mutex Pointer to the mutex.
 * @return 0 on success; non-zero on failure.
 */
int mutex_lock_shared(Mutex *mutex);

/**
 * @brief Releases a mutex lock.
 *
 * @param mutex Pointer to the mutex.
 * @return 0 on success; non-zero on failure.
 */
int mutex_unlock(Mutex *mutex);

/**
 * @brief Destroys a mutex.
 *
 * @param mutex Pointer to the mutex to destroy.
 * @return 0 on success; non-zero on failure.
 */
int mutex_destroy(Mutex *mutex);

/**
 * @brief Executes a callback exactly once for a given once token.
 *
 * @param once Pointer to the once token.
 * @param callback Function to execute once.
 * @return 0 on success; non-zero on failure.
 */
int mutex_once(MutexOnce *once, void (*callback)(void));

#pragma endregion

#pragma region Process Statistics

/**
 * @brief Process resource usage statistics.
 */
struct process_stats {
    double max_rss_mb;             /**< Peak resident memory in megabytes. */
    double current_rss_mb;         /**< Current resident memory in megabytes, or 0 if unsupported. */

    double user_cpu_sec;           /**< User CPU time in seconds. */
    double system_cpu_sec;         /**< Kernel/system CPU time in seconds. */

    long minor_page_faults;        /**< Number of minor page faults. */
    long major_page_faults;        /**< Number of major page faults. */

    long voluntary_ctx_switches;   /**< Number of voluntary context switches. */
    long involuntary_ctx_switches; /**< Number of involuntary context switches. */
};

/**
 * @brief Collects process resource usage statistics.
 *
 * @param out Pointer to the output statistics structure.
 * @return 0 on success; non-zero on failure.
 */
int process_stats_collect(struct process_stats *out);

/**
 * @brief Prints current process resource usage statistics.
 */
void process_stats_print(void);

#pragma endregion
