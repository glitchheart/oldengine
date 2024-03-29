#ifndef LINUX_THREADING
#define LINUX_THREADING

#include <pthread.h>
#include <semaphore.h>

// Define platform-specific functions
#define SemaphoreHandle sem_t
#define WRITE_BARRIER __asm__ __volatile__ ("" ::: "memory"); _mm_sfence()
#define THREAD_PROC(name) static void* name(void* parameters)
typedef void* (*ThreadProc)(void* parameters);

static inline u32 interlocked_compare_exchange(u32 volatile *ptr, u32 next, u32 original)
{
    return __sync_val_compare_and_swap(ptr, original, next);
}

static inline void interlocked_increment(u32 volatile *ptr)
{
    __sync_fetch_and_add(ptr, 1);
}

static inline void release_semaphore(SemaphoreHandle& semaphore_handle)
{
    sem_post(&semaphore_handle);
}

static inline void wait_for_semaphore(SemaphoreHandle& semaphore_handle)
{
    sem_wait(&semaphore_handle);
}

static inline SemaphoreHandle create_semaphore(u32 initial_count, u32 thread_count)
{
    sem_t sem;
    sem_init(&sem, thread_count, initial_count);
    return(sem);
}

static inline void create_thread(ThreadProc thread_proc, void *parameter)
{
    pthread_attr_t attr;
    pthread_t thread;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    pthread_create(&thread, &attr, thread_proc, parameter);
    pthread_attr_destroy(&attr);
}

#include "threading.h"
#include "threading.cpp"

#endif
