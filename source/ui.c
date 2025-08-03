#include <stdio.h>
#include "ui.h"
#include "utils.h"

void printSetupScreen(const u32 n, const bool logging) {
    consoleClear();
    printf("\n\nPress D-Right to increase, D-Left to decrease\n");
    printf("Press A to start\n");
    printf("Press Plus to exit\n");
    printf("Press Y to toggle logging (%s)\n", logging ? "ON" : "OFF");
    printf("Iterations: %d\n", n);
    consoleUpdate(NULL);
}

int updateUserInputs(const u64 kDown, u32 *n, bool *logging, bool *setup) {
    if ((kDown & HidNpadButton_Left) && *n > N_MIN) {
        *n /= 10;
        printSetupScreen(*n, *logging);
    } else if ((kDown & HidNpadButton_Right) && *n < N_MAX) {
        *n *= 10;
        printSetupScreen(*n, logging);
    } else if (kDown & HidNpadButton_Y) {
        *logging = !(*logging);
        printSetupScreen(*n, *logging);
    } else if (kDown & HidNpadButton_A) {
        *setup = false;
        return START_BENCH;
    } else if (kDown & HidNpadButton_Plus) {
        exitCleanly();
        return EXIT;
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
            } else if (kDown & HidNpadButton_Plus) {
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
