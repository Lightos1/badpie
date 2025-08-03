#include "pi.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

bool calculatePI(const bool logging, const u32 n) {
    u32 total = 0;
    const u64 start = armGetSystemTick();

    consoleClear();
    consoleUpdate(NULL);

    /* This method of calculating pi is terrible, but it's bad enough so that precision doesn't run out too early. */
    /* I do not want to deal with types above double yet. */
    /* Don't kill me for this, it works ok and is not too optimal unlike the other method. */
    for (u32 i = 0; i < n; i++) {
        if (logging && ((i + 1) % 10000 == 0 || i == n - 1)) {
            printf("Loop: %d\n", i + 1);
            consoleUpdate(NULL);
        }

        const double x = ((double) rand() / RAND_MAX) * 2.0 - 1.0;
        const double y = ((double) rand() / RAND_MAX) * 2.0 - 1.0;

        if ((x*x) + (y*y) <= 1.0) {
            total++;
        }
    }

    const u64 end = armGetSystemTick();
    const u64 elapsedNs = armTicksToNs(end - start);
    const double duration = elapsedNs / 1e9;

    const double result = ((double) total / n) * 4.0;

    printf("\n\nPI: %f\n", result);
    printf("Time taken: %.2f seconds\n", duration);

    consoleUpdate(NULL);
    return true;
}
