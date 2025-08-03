#include "pi.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <switch.h>

volatile u32 total = 0;
Mutex totalMutex;

void threadFunction(void *arg) {
    const u32 n = *(u32 *) arg;
    u32 localTotal = 0;

    for (u32 i = 0; i < n; i++) {
        const double x = ((double) rand() / RAND_MAX) * 2.0 - 1.0;
        const double y = ((double) rand() / RAND_MAX) * 2.0 - 1.0;

        if ((x * x) + (y * y) <= 1.0) {
            localTotal++;
        }
    }

    mutexLock(&totalMutex);
    total += localTotal;
    mutexUnlock(&totalMutex);
}

bool calculatePI(const u32 n, const u32 threadCount) {
    printf("Calculating PI with %d threads and %d iterations...\n", threadCount, n);
    consoleUpdate(NULL);
    total = 0;

    mutexInit(&totalMutex);
    Thread threads[threadCount];
    u32 thread_args[threadCount];
    const u32 iterationsPerThread = n / threadCount;

    const u64 start = armGetSystemTick();
    for (u32 i = 0; i < threadCount; i++) {
        thread_args[i] = iterationsPerThread;

        threadCreate(&threads[i], threadFunction, &thread_args[i],NULL, 0x4000, 0x2B, -2);
        threadStart(&threads[i]);
    }

    for (u32 i = 0; i < threadCount; i++) {
        threadWaitForExit(&threads[i]);
        threadClose(&threads[i]);
    }

    const u64 end = armGetSystemTick();

    memset(&totalMutex, 0, sizeof(Mutex));
    consoleClear();
    consoleUpdate(NULL);

    const u64 elapsedNs = armTicksToNs(end - start);
    const double duration = elapsedNs / 1e9;

    const double result = ((double) total / n) * 4.0;

    printf("\n\nPI: %f\n", result);
    printf("Time taken: %.2f seconds\n", duration);

    consoleUpdate(NULL);
    return true;
}
