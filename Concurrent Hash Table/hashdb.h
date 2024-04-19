#ifndef HASHDB_H
#define HASHDB_H

#include <stdint.h>

typedef struct hash_struct {
    uint32_t hash;
    char name[50];
    uint32_t salary;
    struct hash_struct next;
} hashRecord;

typedef struct list_struct {
    hashRecord *head;
    pthread_rwlock_t lock;
} list;

void init_hash_table();
void insert(charname, uint32_t salary);
void delete(char name);
hashRecord search(char *name);
void print_all();
void destroy_hash_table();

#endif