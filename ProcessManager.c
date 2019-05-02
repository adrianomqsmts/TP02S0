#include "ProcessManager.h"
#include "Process.h"

void inicializarEstruturas(RunningState *runningState, ReadyState *readyState, BlockedState *blockedState,
                           PcbTable *pcbTable, Cpu *cpu, Time *time) {
    runningState->iPcbTable = -1;
    FFVaziaReady(readyState);
    FFVaziaBlocked(blockedState);
    FLVaziaPcbTable(pcbTable);
    time->time = 0;
}

Processo criarPrimeiroSimulado(Programa *programa, int qtdeInstrucoes) {
    Processo processo;
    processo.pid = 0;
    processo.pid_pai = getpid();
    strcpy(processo.estado, "PRONTO");
    processo.prioridade = 3;
    processo.tempoCPU = 0;
    processo.timeInicio = 0;
    processo.estadoProcesso.inteiro = 0;
    processo.estadoProcesso.contador = 0;
    processo.estadoProcesso.tamanho = qtdeInstrucoes;
    //alocarEstadoPrograma(&processo.estadoProcesso);
    for (int k = 0; k < qtdeInstrucoes; k++) {
        strcpy(processo.estadoProcesso.programa[k].instrucao, programa->instrucoes[k].instrucao);
    }
    return processo;
}

void colocarProcessoCPU(Cpu *cpu, PcbTable *pcbTable, int qtdeInstrucoes) {

    FFVazia(&cpu->programa);

    cpu->programa.tamanho = qtdeInstrucoes;

    for (int i = 0; i < cpu->programa.tamanho; i++) {
        Enfileira(&cpu->programa, pcbTable->vetor[pcbTable->Ultimo - 2].estadoProcesso.programa[i].instrucao);
    }

    cpu->contadorProgramaAtual = pcbTable->vetor[pcbTable->Ultimo - 2].estadoProcesso.contador;
    cpu->fatiaTempo = 10;
    cpu->fatiaTempoUsada = 0;
    cpu->valorInteiro = pcbTable->vetor[pcbTable->Ultimo - 2].estadoProcesso.inteiro;

    strcpy(pcbTable->vetor[pcbTable->Ultimo - 2].estado, "EXECUTANDO");
}

void ImprimirCPU(Cpu *cpu) {

    printf("\nInformacoes CPU: \n");

    printf("\nPrograma na CPU: \n");
    for (int i = 0; i < cpu->programa.tamanho; i++)
        printf("%s", cpu->programa.instrucoes[i].instrucao);
    printf("\n");
    printf("Contador de Programa Atual: %d\n", cpu->contadorProgramaAtual);
    printf("Valor Inteiro: %d\n", cpu->valorInteiro);
    printf("Fatia de Tempo Disponivel: %f\n", cpu->fatiaTempo);
    printf("Fatia de Tempo Usada: %f\n", cpu->fatiaTempoUsada);
    printf("\n\n");
}

void runCPU(Cpu *cpu, Time *time, PcbTable *pcbTable) {

    executarInstrucao(cpu, time);

    /* Atualizando processo simulado */
    pcbTable->vetor[pcbTable->Ultimo - 2].estadoProcesso.inteiro = cpu->valorInteiro;
    pcbTable->vetor[pcbTable->Ultimo - 2].estadoProcesso.contador = cpu->contadorProgramaAtual;
    pcbTable->vetor[pcbTable->Ultimo - 2].tempoCPU = time->time;
}

// Implementação Fila Arranjo

void FFVaziaReady(ReadyState *readyState) {
    readyState->Frente = 1;
    readyState->Tras = readyState->Frente;
}

void FFVaziaBlocked(BlockedState *blockedState) {
    blockedState->Frente = 1;
    blockedState->Tras = blockedState->Frente;
}

int VaziaReady(ReadyState *readyState) { return (readyState->Frente == readyState->Tras); }

int VaziaBlocked(BlockedState *blockedState) { return (blockedState->Frente == blockedState->Tras); }

void EnfileiraReady(ReadyState *readyState, Processo processo) {
    if (readyState->Tras % MAXTAM + 1 == readyState->Frente)
        printf(" Erro fila esta cheia\n");
    else {
        readyState->vetor[readyState->Tras - 1] = processo;
        readyState->Tras = readyState->Tras % MAXTAM + 1;
    }
}

void EnfileiraBlocked(BlockedState *blockedState, Processo processo) {
    if (blockedState->Tras % MAXTAM + 1 == blockedState->Frente)
        printf(" Erro   fila est  a  cheia\n");
    else {
        blockedState->vetor[blockedState->Tras - 1] = processo;
        blockedState->Tras = blockedState->Tras % MAXTAM + 1;
    }
}

void DesenfileiraReady(ReadyState *readyState, Processo *processo) {
    if (VaziaReady(readyState))
        printf("Erro fila esta vazia\n");
    else {
        *processo = readyState->vetor[readyState->Frente - 1];
        readyState->Frente = readyState->Frente % MAXTAM + 1;
    }
}

void DesenfileiraBlocked(BlockedState *blockedState, Processo *processo) {
    if (VaziaBlocked(blockedState))
        printf("Erro fila esta vazia\n");
    else {
        *processo = blockedState->vetor[blockedState->Frente - 1];
        blockedState->Frente = blockedState->Frente % MAXTAM + 1;
    }
}


void ImprimeReady(ReadyState *readyState) {
    int Aux;
    for (Aux = readyState->Frente - 1; Aux <= (readyState->Tras - 2); Aux++)
        printf("%12d\n", readyState->vetor[Aux]);
}

void ImprimeBlocked(BlockedState *blockedState) {
    int Aux;
    for (Aux = blockedState->Frente - 1; Aux <= (blockedState->Tras - 2); Aux++)
        printf("%12d\n", blockedState->vetor[Aux]);
}

// Implementação Lista Arranjo

void FLVaziaPcbTable(PcbTable *pcbTable) {
    pcbTable->Primeiro = 1;
    pcbTable->Ultimo = pcbTable->Primeiro;
}


int VaziaPcbTable(PcbTable *pcbTable) {
    return (pcbTable->Primeiro == pcbTable->Ultimo);
}

void InserePcbTable(PcbTable *pcbTable, Processo processo) {
    if (pcbTable->Ultimo > MAXTAM) printf("Lista esta cheia\n");
    else {
        pcbTable->vetor[pcbTable->Ultimo - 1] = processo;
        pcbTable->Ultimo++;
    }
}

void RetiraPcbTable(PcbTable *pcbTable, int indice, Processo *processo) {
    int Aux;

    if (VaziaPcbTable(pcbTable) || indice >= pcbTable->Ultimo) {
        printf(" Erro   Posicao nao existe\n");
        return;
    }
    *processo = pcbTable->vetor[indice - 1];
    pcbTable->Ultimo--;
    for (Aux = indice; Aux < pcbTable->Ultimo; Aux++)
        pcbTable->vetor[Aux - 1] = pcbTable->vetor[Aux];
}

void ImprimePcbTable(PcbTable *pcbTable) {
    int Aux;

    printf("\nLista de Processos:\n\n");
    for (Aux = pcbTable->Primeiro - 1; Aux <= (pcbTable->Ultimo - 2); Aux++) {
        printf("PID: %i\n", pcbTable->vetor[Aux].pid);
        printf("PID Pai: %i\n", pcbTable->vetor[Aux].pid_pai);
        printf("Estado: %s\n", pcbTable->vetor[Aux].estado);
        printf("Tempo CPU: %f\n", pcbTable->vetor[Aux].tempoCPU);
        printf("Tempo Inicio: %f\n", pcbTable->vetor[Aux].timeInicio);
        printf("Prioridade: %d\n", pcbTable->vetor[Aux].prioridade);
        printf("Valor Inteiro: %d\n", pcbTable->vetor[Aux].estadoProcesso.inteiro);
        printf("Contador de Programa: %d\n", pcbTable->vetor[Aux].estadoProcesso.contador);
        printf("Programa: \n");
        for (int i = 0; i < pcbTable->vetor[Aux].estadoProcesso.tamanho; i++)
            printf("%s", pcbTable->vetor[Aux].estadoProcesso.programa[i].instrucao);
        printf("\n\n");
    }

}
