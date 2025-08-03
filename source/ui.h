#pragma once
#include <switch.h>

#define EXIT 0
#define LOOP 1
#define START_BENCH 2
#define N_MIN_ITERATIONS 100
#define N_MAX_ITERATIONS 1000000000
#define MAX_THREADS 4

void printSetupScreen(u32, u32);
int updateUserInputs(u64, u32*, bool*, u8*);
int finishedLoopUI(bool*, bool*, PadState*);
