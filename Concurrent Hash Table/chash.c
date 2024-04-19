#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include "hashdb.h"
#include "rwlocks.h"
#include "common_threads.h"

int main() {
    FILE *infile = fopen("command.txt", "r");
    FILE *outfile = fopen("output.txt", "w");

    init_hash_table();
    rwlock_init(&mutex);

    int threads;
    fscanf(infile, "threads,%d,0\n", &threads);

    pthread_t *threadArray = malloc(threads * sizeof(pthread_t));
    Command *commands = malloc(threads * sizeof(Command));

    for (int i = 0; i < threads; i++) {
        fscanf(infile, "%[^,],%[^,],%u\n", commands[i].command, commands[i].name, &commands[i].salary);
        commands[i].outfile = outfile;
        pthread_create(&threadArray[i], NULL, threadFunction, (void *)&commands[i]);
    }

    for (int i = 0; i < threads; i++) {
        pthread_join(threadArray[i], NULL);
    }

    printf("Number of lock acquisitions: %d", aquire);
    printf("Number of lock releases: %d", release);

    // Cleanup
    free(threadArray);
    free(commands);
    fclose(infile);
    fclose(outfile);
    return 0;
}