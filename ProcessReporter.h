#ifndef TP02SO_PROCESSREPORTER_H
#define TP02SO_PROCESSREPORTER_H

#include "ProcessManager.h"

void ImprimeFilaReporter(Fila *fila, PcbTable *pcbTable);
void imprimeReporter(Time *time, PcbTable *pcbTable, RunningState *runningState, BlockedState *blockedState,
                     ReadyState *readyState);

#endif //TP02SO_PROCESSREPORTER_H
