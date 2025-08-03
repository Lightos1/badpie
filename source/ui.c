#include <stdio.h>
#include "ui.h"
#include "utils.h"

void printSetupScreen(const u32 n, const u32 threadCount) {
    consoleClear();
    printf("\n\nPress D-Right to increase iterations, D-Left to decrease\n");
    printf("Press A to start\n");
    printf("Press Plus to exit\n");
    printf("Iterations: %d\n", n);
    printf("Threads: %d (Increase with Y)\n", threadCount);
    consoleUpdate(NULL);
}

int updateUserInputs(const u64 kDown, u32 *n, bool *setup, u8 *threadCount) {
    if ((kDown & HidNpadButton_Left) && *n > N_MIN_ITERATIONS) {
        *n /= 10;
        printSetupScreen(*n, *threadCount);
    } else if ((kDown & HidNpadButton_Right) && *n < N_MAX_ITERATIONS) {
        *n *= 10;
        printSetupScreen(*n, *threadCount);
    } else if (kDown & HidNpadButton_A) {
        *setup = false;
        return START_BENCH;
    } else if (kDown & HidNpadButton_Plus) {
        exitCleanly();
        return EXIT;
    } else if (kDown & HidNpadButton_Y) {
        if (*threadCount < MAX_THREADS) {
            (*threadCount)++;
        } else {
            *threadCount = 1;
        }
        printSetupScreen(*n, *threadCount);
    }

    svcSleepThread(1000000);
    return LOOP;
}

int finishedLoopUI(bool *finished, bool *setup, PadState *pad) {
    if (*finished) {
        printf("\n\nPress A to retry, plus to exit, X to go back to config\n");
        consoleUpdate(NULL);

        while (true) {
            padUpdate(pad);
            const u64 kDown = padGetButtonsDown(pad);

            if (kDown & HidNpadButton_A) {
                *finished = false;
                consoleClear();
                break;
            }

            if (kDown & HidNpadButton_Plus) {
                exitCleanly();
                return 0;
            }

            if (kDown & HidNpadButton_X) {
                *setup = true;
                *finished = false;
                consoleClear();
                break;
            }

            svcSleepThread(10000000);
        }
    }
    return 1;
}
