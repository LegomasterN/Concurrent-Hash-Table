#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include "hashdb.h"
#include "rwlocks.h"
#include "common_threads.h"

void *threadFunction(void *arg) {
    Command *cmd = (Command *)arg;
    processCommand(cmd->command, cmd->name, cmd->salary, cmd->outfile);
    pthread_exit(NULL);
}

void processCommand(char *command, char *param1, uint32_t param2, FILE *outputFile) {
    if (strcmp(command, "insert") == 0) {
        insert(param1, param2);
        fprintf(outputFile, "INSERT,%s,%u\n", param1, param2);
    } else if (strcmp(command, "delete") == 0) {
        delete(param1);
        fprintf(outputFile, "DELETE,%s\n", param1);
    } else if (strcmp(command, "search") == 0) {
        hashRecord *record = search(param1);
        if (record) {
            fprintf(outputFile, "%u,%s,%u\n", record->hash, record->name, record->salary);
        } else {
            fprintf(outputFile, "No Record Found\n");
        }
    } else if (strcmp(command, "print") == 0) {
        print_all();
    }
}

// Jenkins Function
uint32_t jenkins_hash(const void *key, size_t length)
 {
    const uint8_t *data = (const uint8_t *)key;
    uint32_t hash = 0;
    size_t i;

    for (i = 0; i < length; ++i) {
        hash += data[i];
        hash += (hash << 10);
        hash ^= (hash >> 6);
    }
    hash += (hash << 3);
    hash ^= (hash >> 11);
    hash += (hash << 15);

    return hash;
}

// Create a new node and return it
hashRecord* createNode(const char *name, uint32_t salary)
{
    hashRecord* newNode = (hashRecord*)malloc(sizeof(hashRecord));
    if (newNode != NULL) {
        newNode->hash = jenkins_hash(name, strlen(name));
        strcpy(newNode->name, name);
        newNode->salary = salary;
        newNode->next = NULL;
    }
    return newNode;
}

// Function to insert a node in ascending order based on the hash value
void insertNode(list* lst, hashRecord* newNode) 
{
    printf("Thread %lu trying to acquire write lock for insertion\n", pthread_self());
    pthread_rwlock_wrlock(&lst->lock);
    printf("Thread %lu acquired write lock for insertion\n", pthread_self());
    
    hashRecord *current = lst->head;
    hashRecord *prev = NULL;

    // Traverse the list to find the appropriate position for insertion
    while (current != NULL && newNode->hash > current->hash) {
        prev = current;
        current = current->next;
    }

    // Insert the node
    if (prev == NULL) {
        // Insert at the beginning of the list
        newNode->next = lst->head;
        lst->head = newNode;
    } else {
        // Insert after the previous node
        prev->next = newNode;
        newNode->next = current;
    }

    pthread_rwlock_unlock(&lst->lock);
    printf("Thread %lu released write lock for insertion\n", pthread_self());
}

// Function to delete a node by name
void deleteNode(list* lst, const char *name) 
{
    printf("Thread %lu trying to acquire write lock for deletion\n", pthread_self());
    pthread_rwlock_wrlock(&lst->lock);
    printf("Thread %lu acquired write lock for deletion\n", pthread_self());

    hashRecord *current = lst->head;
    hashRecord *prev = NULL;

    // Traverse the list to find the node to delete
    while (current != NULL && strcmp(current->name, name) != 0) {
        prev = current;
        current = current->next;
    }

    // If the node is found, delete it
    if (current != NULL) {
        if (prev == NULL) {
            // If the node to delete is the head of the list
            lst->head = current->next;
        } else {
            // If the node to delete is not the head
            prev->next = current->next;
        }
        free(current);
        printf("Node with name %s deleted\n", name);
    } else {
        printf("Node with name %s not found\n", name);
    }

    pthread_rwlock_unlock(&lst->lock);
    printf("Thread %lu released write lock for deletion\n", pthread_self());
}