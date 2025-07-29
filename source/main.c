#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <switch.h>
#include <time.h>

#define N_MIN 100
#define N_MAX 1000000000

void printSetupScreen(u32 n, bool logging) {
    consoleClear();
    printf("\n\nPress D-Right to increase, D-Left to decrease\n");
    printf("Press A to start\n");
    printf("Press Plus to exit\n");
    printf("Press Y to toggle logging (%s)\n", logging ? "ON" : "OFF");
    printf("Iterations: %d\n", n);
    consoleUpdate(NULL);
}
void exitCleanly() {
    consoleClear();
    consoleUpdate(NULL);
    svcSleepThread(100000000);
    consoleExit(NULL);
}

int main() {
    consoleInit(NULL);

    padConfigureInput(1, HidNpadStyleSet_NpadStandard);

    PadState pad;
    padInitializeDefault(&pad);
    srand(time(NULL));
    bool finished = false;
    bool setup = true;
    bool logging = true;
    u32 n = 1000000;

    while (appletMainLoop()) {

        if (setup) {
            printSetupScreen(n, logging);

            consoleUpdate(NULL);

            while (true) {
                padUpdate(&pad);
                u64 kDown = padGetButtonsDown(&pad);

                if ((kDown & HidNpadButton_Left) && n > N_MIN) {
                    n /= 10;

                    printSetupScreen(n, logging);
                } else if ((kDown & HidNpadButton_Right) && n < N_MAX) {
                    n *= 10;

                    printSetupScreen(n, logging);
                } else if (kDown & HidNpadButton_Y) {
                    logging = !logging;
                    printSetupScreen(n, logging);
                } else if (kDown & HidNpadButton_A) {
                    setup = false;
                    break;
                } else if (kDown & HidNpadButton_Plus) {
                    exitCleanly();
                    return 0;
                }
                svcSleepThread(1000000);
            }
        }

        padUpdate(&pad);

        u64 kDown = padGetButtonsDown(&pad);

        if (kDown & HidNpadButton_Plus) {
            break;
        }

        if (finished) {
            printf("\n\nPress A to retry, plus to exit, X to go back to config\n");
            consoleUpdate(NULL);

            while (true) {
                padUpdate(&pad);
                u64 kDown = padGetButtonsDown(&pad);

                if (kDown & HidNpadButton_A) {
                    finished = false;
                    consoleClear();
                    break;
                } else if (kDown & HidNpadButton_Plus) {
                    exitCleanly();
                    return 0;
                } else if (kDown & HidNpadButton_X) {
                    setup = true;
                    finished = false;
                    consoleClear();
                    break;
                }
                svcSleepThread(10000000);
            }
        }

        if (setup) {
            continue;
        }

        u32 total = 0;
        u64 start = armGetSystemTick();

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

            double x = ((double) rand() / RAND_MAX) * 2.0 - 1.0;
            double y = ((double) rand() / RAND_MAX) * 2.0 - 1.0;

            if ((x*x) + (y*y) <= 1.0) {
                total++;
            }
        }

        u64 end = armGetSystemTick();
        u64 elapsedNs = armTicksToNs(end - start);
        double duration = elapsedNs / 1e9;

        double result = ((double) total / n) * 4.0;

        printf("\n\nPI: %f\n", result);
        printf("Time taken: %.2f seconds\n", duration);
        finished = true;
        consoleUpdate(NULL);
    }
}
