#include <switch.h>
#include "main.h"

#include <math.h>
#include <stdlib.h>

int main() {
    consoleInit(NULL);
    const u64 ticks = armGetSystemTick();
    srand((unsigned int)(ticks & 0xFFFFFFFF));


    padConfigureInput(1, HidNpadStyleSet_NpadStandard);
    PadState pad;
    padInitializeDefault(&pad);

    bool finished = false;
    bool setup = true;

    u32 n = 1000000;

    while (appletMainLoop()) {
        bool logging = false;
        if (setup) {
            printSetupScreen(n, logging);
            while (true) {
                padUpdate(&pad);
                const u64 kDown = padGetButtonsDown(&pad);

                const u32 result = updateUserInputs(kDown, &n, &logging, &setup);

                if (result == START_BENCH) {
                    break;
                }

                if (!result) {
                    return 0;
                }
            }
        }

        padUpdate(&pad);
        const u64 kDown = padGetButtonsDown(&pad);

        if (kDown & HidNpadButton_Plus) {
            break;
        }

        if (!finishedLoopUI(&finished, &setup, &pad)) {
            return 0;
        }

        if (setup) {
            continue;
        }

        finished = calculatePI(logging, n);
    }

    return 1;
}
