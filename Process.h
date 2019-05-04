#ifndef TP02SO_PROCESS_H
#define TP02SO_PROCESS_H

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "ProcessManager.h"

void executarInstrucao(Cpu *cpu, Time *time, RunningState *runningState, PcbTable *pcbTable, BlockedState *blockedState, ReadyState *readyState, Processo *processo);

#endif //TP02SO_PROCESS_H
