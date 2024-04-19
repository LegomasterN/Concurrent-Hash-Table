#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>

#include "rwlocks.h"
#include "common.h"
#include "common_threads.h"

#ifdef linux
#include <semaphore.h>
#elif _APPLE
#include "zemaphore.h"
#endif

void rwlock_init(rwlock_t lock) 
{
    lock->readers = 0;
    Sem_init(&lock->lock, 1); 
    Sem_init(&lock->writelock, 1); 
}

void rwlock_acquire_readlock(rwlock_tlock) 
{
    Sem_wait(&lock->lock);
    lock->readers++;
    if (lock->readers == 1)
    Sem_wait(&lock->writelock);
    Sem_post(&lock->lock);
}

rwlocks.c
void rwlock_release_readlock(rwlock_t lock) 
{
    Sem_wait(&lock->lock);
    lock->readers--;
    if (lock->readers == 0)
    Sem_post(&lock->writelock);
    Sem_post(&lock->lock);
}

void rwlock_acquire_writelock(rwlock_tlock) {
    Sem_wait(&lock->writelock);
}

void rwlock_release_writelock(rwlock_t lock) {
    Sem_post(&lock->writelock);
}

int counter = 0;
int read_loops;
int write_loops;

rwlock_t mutex;

voidreader() {
    int i;
    int local = 0;
    for (i = 0; i < read_loops; i++) 
    {
        rwlock_acquire_readlock(&mutex);
        local = counter;
        rwlock_release_readlock(&mutex);
        printf("read %d\n", local);
    }
    printf("read done: %d\n", local);
    return NULL;
}

voidwriter(void *arg) 
{
    int i;
    for (i = 0; i < write_loops; i++) 
    {
        rwlock_acquire_writelock(&mutex);
        counter++;
        rwlock_release_writelock(&mutex);
    }
    printf("write done\n");
    return NULL;
} 
