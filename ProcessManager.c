#include "ProcessManager.h"
#include "Process.h"

void inicializarEstruturas(RunningState *runningState, ReadyState *readyState, BlockedState *blockedState,
                           PcbTable *pcbTable, Cpu *cpu, Time *time) {
    runningState->iPcbTable = 0;
    FFVaziaReady(readyState);
    FFVaziaBlocked(blockedState);
    FLVaziaPcbTable(pcbTable);
    FFVazia(&cpu->programa);
    cpu->fatiaTempoUsada = 0;
    cpu->fatiaTempo = 0;
    cpu->contadorProgramaAtual = 0;
    cpu->valorInteiro = 0;
    time->time = 0;
}

Processo criarPrimeiroSimulado(Programa *programa, Time *timee, int qtdeInstrucoes, int pidPai) {
    Processo processo;

    srand(time(NULL));

    processo.pid = 0;
    processo.pid_pai = pidPai;
    processo.prioridade = 0;
    processo.tempoCPU = 0;
    processo.timeInicio = timee->time;
    processo.estadoProcesso.inteiro = 0;
    processo.estadoProcesso.contador = 0;
    processo.estadoProcesso.tamanho = qtdeInstrucoes;
    //alocarEstadoPrograma(&processo.estadoProcesso);
    for (int k = 0; k < qtdeInstrucoes; k++) {
        strcpy(processo.estadoProcesso.programa[k].instrucao, programa->instrucoes[k].instrucao);
    }
    strcpy(processo.estado, "PRONTO");
    return processo;
}

Processo criarProcessoSimulado(Time *timee, Processo *processoPai) {
    Processo processo;

    srand(time(NULL));

    processo.pid = processoPai->pid + 1; // Ta errado. Arrumar isso.
    processo.pid_pai = processoPai->pid;
    processo.prioridade = processoPai->prioridade;
    processo.tempoCPU = 0;
    processo.timeInicio = timee->time;
    processo.estadoProcesso.inteiro = processoPai->estadoProcesso.inteiro;
    processo.estadoProcesso.contador = processoPai->estadoProcesso.contador + 1;
    processo.estadoProcesso.tamanho = processoPai->estadoProcesso.tamanho;
    //alocarEstadoPrograma(&processo.estadoProcesso);
    for (int i = 0; i < processoPai->estadoProcesso.tamanho; i++) {
        strcpy(processo.estadoProcesso.programa[i].instrucao, processoPai->estadoProcesso.programa[i].instrucao);
    }
    strcpy(processo.estado, "PRONTO");
    return processo;
}

Processo colocarProcessoCPU(Cpu *cpu, ReadyState *readyState) {

    Processo processo;

    DesenfileiraReady(readyState, &processo);

    cpu->programa.tamanho = processo.estadoProcesso.tamanho;

    for (int i = 0; i < cpu->programa.tamanho; i++) {
        EnfileiraPrograma(&cpu->programa, processo.estadoProcesso.programa[i].instrucao);
    }

    cpu->contadorProgramaAtual = processo.estadoProcesso.contador;
    cpu->fatiaTempo = 10;
    cpu->fatiaTempoUsada = 0;
    cpu->valorInteiro = processo.estadoProcesso.inteiro;

    return processo;
}

void ImprimirCPU(Cpu *cpu) {

    printf("\nInformacoes CPU: \n");

    printf("\nPrograma na CPU: \n");
    for (int i = 0; i < cpu->programa.tamanho; i++)
        printf("%s", cpu->programa.instrucoes[i].instrucao);
    printf("\n");
    printf("Contador de Programa Atual: %d\n", cpu->contadorProgramaAtual);
    printf("Valor Inteiro: %d\n", cpu->valorInteiro);
    printf("Fatia de Tempo Disponivel: %d\n", cpu->fatiaTempo);
    printf("Fatia de Tempo Usada: %d\n", cpu->fatiaTempoUsada);
    printf("\n\n");
}

void runCPU(Cpu *cpu, Time *time, PcbTable *pcbTable, RunningState *runningState, BlockedState *blockedState,
            ReadyState *readyState, Processo *processo) {

    strcpy(processo->estado, "EM EXECUCAO");

    executarInstrucao(cpu, time, runningState, pcbTable, blockedState, readyState, processo);

    switch (processo->prioridade){
        case 0:
            cpu->fatiaTempoUsada += 1; break;
        case 1:
            cpu->fatiaTempoUsada += 2; break;
        case 2:
            cpu->fatiaTempoUsada += 4; break;
        case 3:
            cpu->fatiaTempoUsada += 8; break;
        default:
            printf("Erro ao atualizar fatia de tempo CPU!\n");
    }

    /* Atualizando processo simulado */
    processo->estadoProcesso.inteiro = cpu->valorInteiro;
    processo->estadoProcesso.contador = cpu->contadorProgramaAtual;
    processo->tempoCPU = cpu->fatiaTempoUsada; // Ta certo?
    for (int i = 0; i < processo->estadoProcesso.tamanho; i++) { // Da errado quando o programa da CPU é maior que o programa do processo.
        strcpy(processo->estadoProcesso.programa[i].instrucao, cpu->programa.instrucoes[i].instrucao);
    }
    pcbTable->vetor[runningState->iPcbTable] = *processo;

    // Usar troca de contexto quando processo terminar execução.

    // Isso eh realmente aqui?
    if(cpu->fatiaTempoUsada >= cpu->fatiaTempo) // Fatia Tempo Usada pode ultrapassar Fatia Tempo?
        EnfileiraBlocked(blockedState, processo);
}

// Implementação Fila Arranjo

void FFVaziaReady(ReadyState *readyState) {
    readyState->Frente = 0;
    readyState->Tras = readyState->Frente;
}

void FFVaziaBlocked(BlockedState *blockedState) {
    blockedState->Frente = 0;
    blockedState->Tras = blockedState->Frente;
}

int VaziaReady(ReadyState *readyState) { return (readyState->Frente == readyState->Tras); }

int VaziaBlocked(BlockedState *blockedState) { return (blockedState->Frente == blockedState->Tras); }

void EnfileiraReady(ReadyState *readyState, Processo *processo) {
    if (readyState->Tras % MAXTAM + 1 == readyState->Frente)
        printf("Erro fila Ready esta cheia\n");
    else {
        strcpy(processo->estado, "PRONTO");
        readyState->vetor[readyState->Tras] = *processo;
        printf("\nProcesso de PID %i adicionado a FILA PRONTO!\n", processo->pid);
        readyState->Tras = readyState->Tras % MAXTAM + 1;
    }
}

void EnfileiraBlocked(BlockedState *blockedState, Processo *processo) {

    if (blockedState->Tras % MAXTAM + 1 == blockedState->Frente)
        printf("\nErro fila Blocked esta  cheia!\n");
    else {
        strcpy(processo->estado, "BLOQUEADO");
        blockedState->vetor[blockedState->Tras] = *processo;
        printf("\nProcesso de PID %i adicionado a FILA BLOQUEADO!\n", processo->pid);
        blockedState->Tras = blockedState->Tras % MAXTAM + 1;
    }
}

void DesenfileiraReady(ReadyState *readyState, Processo *processo) {
    if (VaziaReady(readyState))
        printf("\nErro fila Ready esta vazia\n");
    else {
        *processo = readyState->vetor[readyState->Frente];
        readyState->Frente = readyState->Frente % MAXTAM + 1;
    }
}

int DesenfileiraBlocked(BlockedState *blockedState, Processo *processo) {
    if (VaziaBlocked(blockedState)){
        printf("\nErro fila Blocked esta vazia\n");
        return 0;
    }
    else {
        *processo = blockedState->vetor[blockedState->Frente];
        blockedState->Frente = blockedState->Frente % MAXTAM + 1;
        return 1;
    }
}


void ImprimeReady(ReadyState *readyState) {
    int Aux;
    printf("Fila de Processsos Ready:\n\n");
    for (Aux = readyState->Frente; Aux <= (readyState->Tras - 1); Aux++){
        printf("PID: %i\n", readyState->vetor[Aux].pid);
        printf("PID Pai: %i\n", readyState->vetor[Aux].pid_pai);
        printf("Estado: %s\n", readyState->vetor[Aux].estado);
        printf("Tempo CPU: %d\n", readyState->vetor[Aux].tempoCPU);
        printf("Tempo Inicio: %d\n", readyState->vetor[Aux].timeInicio);
        printf("Prioridade: %d\n", readyState->vetor[Aux].prioridade);
        printf("Valor Inteiro: %d\n", readyState->vetor[Aux].estadoProcesso.inteiro);
        printf("Contador de Programa: %d\n", readyState->vetor[Aux].estadoProcesso.contador);
        printf("Programa: \n");
        for (int i = 0; i < readyState->vetor[Aux].estadoProcesso.tamanho; i++)
            printf("%s", readyState->vetor[Aux].estadoProcesso.programa[i].instrucao);
        printf("\n");
    }
}

void ImprimeBlocked(BlockedState *blockedState) {
    int Aux;
    printf("Fila de Processsos Blocked:\n\n");
    for (Aux = blockedState->Frente; Aux <= (blockedState->Tras - 1); Aux++){
        printf("PID: %i\n", blockedState->vetor[Aux].pid);
        printf("PID Pai: %i\n", blockedState->vetor[Aux].pid_pai);
        printf("Estado: %s\n", blockedState->vetor[Aux].estado);
        printf("Tempo CPU: %d\n", blockedState->vetor[Aux].tempoCPU);
        printf("Tempo Inicio: %d\n", blockedState->vetor[Aux].timeInicio);
        printf("Prioridade: %d\n", blockedState->vetor[Aux].prioridade);
        printf("Valor Inteiro: %d\n", blockedState->vetor[Aux].estadoProcesso.inteiro);
        printf("Contador de Programa: %d\n", blockedState->vetor[Aux].estadoProcesso.contador);
        printf("Programa: \n");
        for (int i = 0; i < blockedState->vetor[Aux].estadoProcesso.tamanho; i++)
            printf("%s", blockedState->vetor[Aux].estadoProcesso.programa[i].instrucao);
        printf("\n");
    }
}

// Implementação Lista Arranjo

void FLVaziaPcbTable(PcbTable *pcbTable) {
    pcbTable->Primeiro = 0;
    pcbTable->Ultimo = pcbTable->Primeiro;
}


int VaziaPcbTable(PcbTable *pcbTable) {
    return (pcbTable->Primeiro == pcbTable->Ultimo);
}

void InserePcbTable(PcbTable *pcbTable, Processo processo) {
    if (pcbTable->Ultimo > MAXTAM) printf("Lista esta cheia\n");
    else {
        pcbTable->vetor[pcbTable->Ultimo] = processo;
        pcbTable->Ultimo++;
    }
}

void RetiraPcbTable(PcbTable *pcbTable, int indice, Processo *processo) {
    int Aux;

    if (VaziaPcbTable(pcbTable) || indice >= pcbTable->Ultimo) {
        printf("Erro Posicao nao existe\n");
        return;
    }
    *processo = pcbTable->vetor[indice];
    pcbTable->Ultimo--;
    for (Aux = indice; Aux < pcbTable->Ultimo; Aux++)
        pcbTable->vetor[Aux] = pcbTable->vetor[Aux];
}

void ImprimePcbTable(PcbTable *pcbTable) {
    int Aux;

    printf("\nLista de Processos na Tabela:\n\n");
    for (Aux = pcbTable->Primeiro; Aux <= (pcbTable->Ultimo - 1); Aux++) {
        printf("PID: %i\n", pcbTable->vetor[Aux].pid);
        printf("PID Pai: %i\n", pcbTable->vetor[Aux].pid_pai);
        printf("Estado: %s\n", pcbTable->vetor[Aux].estado);
        printf("Tempo CPU: %d\n", pcbTable->vetor[Aux].tempoCPU);
        printf("Tempo Inicio: %d\n", pcbTable->vetor[Aux].timeInicio);
        printf("Prioridade: %d\n", pcbTable->vetor[Aux].prioridade);
        printf("Valor Inteiro: %d\n", pcbTable->vetor[Aux].estadoProcesso.inteiro);
        printf("Contador de Programa: %d\n", pcbTable->vetor[Aux].estadoProcesso.contador);
        printf("Programa: \n");
        for (int i = 0; i < pcbTable->vetor[Aux].estadoProcesso.tamanho; i++)
            printf("%s", pcbTable->vetor[Aux].estadoProcesso.programa[i].instrucao);
        printf("\n\n");
    }

}

void imprimeReporter(Cpu *cpu, Time *time, PcbTable *pcbTable, RunningState *runningState, BlockedState *blockedState,
            ReadyState *readyState, Processo *processo){
    Processo processoSimulado;

    if(runningState->iPcbTable != -1)
        processoSimulado = pcbTable->vetor[runningState->iPcbTable];
    printf("**************************************************\n");
    printf("The current system state is as follows: \n");
    printf("**************************************************\n");
    printf("CURRENT TIME: %d\n", time->time);
    printf("\nCPU PROCESS:\n");
    printf("PID: %d\tPPID: %d\tprioridade: %d\tValue: %d\tStart Time: %d\tCPU Time: %d\n", processoSimulado.pid,
            processoSimulado.pid_pai, processoSimulado.prioridade, cpu->valorInteiro, processoSimulado.timeInicio, processoSimulado.tempoCPU+cpu->fatiaTempoUsada);
    printf("\nRUNNING PROCESS IN PCBTABLE:\n");
    if(runningState->iPcbTable != -1)
        printf("PID: %d\tPPID: %d\tprioridade: %d\tValue: %d\tStart Time: %d\tCPU Time: %d\n", processoSimulado.pid,
                processoSimulado.pid_pai, processoSimulado.prioridade, cpu->valorInteiro, processoSimulado.timeInicio, processoSimulado.tempoCPU);
    else printf("CPU Ociosa\n");
    printf("\nBLOCKED PROCESSES:\n");
    printf("Queue of blocked processes:\n");
    if(VaziaBlocked(&blockedState) == 1){
        printf("FILA VAZIA!\n");
    } else ImprimeBlocked(&blockedState);
    printf("\nPROCESSES READY TO EXECUTE:\n");
    printf("Queue of processes ready:\n");
    if(VaziaReady(&readyState) == 1){
        printf("FILA VAZIA!\n");
    } else ImprimeReady(&readyState);
}
