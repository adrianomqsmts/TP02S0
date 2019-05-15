#include "ProcessReporter.h"
#include "ProcessManager.h"

void imprimeReporter(Time *time, PcbTable *pcbTable, RunningState *runningState, BlockedState *blockedState,
                     ReadyState *readyState) {

    printf("\n\n**************************************************\n");
    printf("The current system state is as follows: \n");
    printf("**************************************************\n");
    printf("CURRENT TIME: %d\n", time->time);
    printf("RUNNING PROCESS:\n");
    if (runningState->iPcbTable != -1) {
        printf("pid %d, ppid %d, priority %d, value %d, start time %d, CPU time used so far %d\n",
               pcbTable->vetor[runningState->iPcbTable].pid,
               pcbTable->vetor[runningState->iPcbTable].pid_pai,
               pcbTable->vetor[runningState->iPcbTable].prioridade,
               pcbTable->vetor[runningState->iPcbTable].estadoProcesso.inteiro,
               pcbTable->vetor[runningState->iPcbTable].timeInicio,
               pcbTable->vetor[runningState->iPcbTable].tempoCPU);
    }
    printf("BLOCKED PROCESSES:\n");
    printf("Queue of blocked processes:\n");
    ImprimeFilaReporter(&blockedState->filaBlockedState, pcbTable);
    printf("PROCESSES READY TO EXECUTE:\n");
    printf("Queue of processes with priority 0:\n");
    ImprimeFilaReporter(&readyState->filaPrioridade0, pcbTable);
    printf("Queue of processes with priority 1:\n");
    ImprimeFilaReporter(&readyState->filaPrioridade1, pcbTable);
    printf("Queue of processes with priority 2:\n");
    ImprimeFilaReporter(&readyState->filaPrioridade2, pcbTable);
    printf("Queue of processes with priority 3:\n");
    ImprimeFilaReporter(&readyState->filaPrioridade3, pcbTable);
    printf("**************************************************\n\n");

}

void ImprimeFilaReporter(Fila *fila, PcbTable *pcbTable) {
    int Aux;
    for (Aux = fila->Frente; Aux <= fila->Tras - 1; Aux++) {
        printf("pid %d, ppid %d, priority %d, value %d, start time %d, CPU time used so far %d\n",
               pcbTable->vetor[fila->vetor[Aux]].pid,
               pcbTable->vetor[fila->vetor[Aux]].pid_pai,
               pcbTable->vetor[fila->vetor[Aux]].prioridade,
               pcbTable->vetor[fila->vetor[Aux]].estadoProcesso.inteiro,
               pcbTable->vetor[fila->vetor[Aux]].timeInicio,
               pcbTable->vetor[fila->vetor[Aux]].tempoCPU);
    }
}