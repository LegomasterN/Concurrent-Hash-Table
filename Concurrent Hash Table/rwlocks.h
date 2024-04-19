#ifndef RWLOCKS_H
#define RWLOCKS_H
#include "common_threads.h"

extern int aquire;
extern int release;

void rwlock_init(rwlock_t lock);
void rwlock_acquire_readlock(rwlock_tlock);
void rwlock_release_readlock(rwlock_t lock);
void rwlock_acquire_writelock(rwlock_tlock);
void rwlock_release_writelock(rwlock_t *lock);

#endif