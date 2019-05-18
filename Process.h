#ifndef TP02SO_PROCESS_H
#define TP02SO_PROCESS_H

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "ProcessManager.h"

int executarInstrucao(Cpu *cpu, Time *time, RunningState *runningState, PcbTable *pcbTable, BlockedState *blockedState,
                      ReadyState *readyState, Processo *processo, Tickets *ticketsSorteados);

#endif //TP02SO_PROCESS_H
