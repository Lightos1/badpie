#include "utils.h"

void exitCleanly() {
    consoleClear();
    consoleUpdate(NULL);
    svcSleepThread(100000000);
    consoleExit(NULL);
}
