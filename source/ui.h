#pragma once
#include <switch.h>

#define EXIT 0
#define LOOP 1
#define START_BENCH 2
#define N_MIN 100
#define N_MAX 1000000000

void printSetupScreen(u32, bool);
int updateUserInputs(u64, u32*, bool*, bool*);
int finishedLoopUI(bool*, bool*, PadState*);
