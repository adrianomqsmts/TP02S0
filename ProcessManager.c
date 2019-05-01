#include "ProcessManager.h"

void inicializarEstruturas() {
    runningState.iPcbTable = -1;
    FFVaziaReady(readyState);
    FFVaziaBlocked(blockedState);
    FLVazia(pcbTable);
    cpu.contadorAtual = 0;
    cpu.n = 0;
    cpu.tempoAtual = 0;
    cpu.tempoCpu = 0;
    timee.time = 0;
}

// Implementação Fila Arranjo

void FFVaziaReady() {
    readyState.Frente = 1;
    readyState.Tras = readyState.Frente;
}

void FFVaziaBlocked() {
    blockedState.Frente = 1;
    blockedState.Tras = blockedState.Frente;
}

int VaziaReady() { return (readyState.Frente == readyState.Tras); }

int VaziaBlocked() { return (blockedState.Frente == blockedState.Tras); }

void EnfileiraReady(Processo processo) {
    if (readyState.Tras % MAXTAM + 1 == readyState.Frente)
        printf(" Erro fila esta cheia\n");
    else {
        readyState.vetor[readyState.Tras - 1] = processo;
        readyState.Tras = readyState.Tras % MAXTAM + 1;
    }
}

void EnfileiraBlocked(Processo processo) {
    if (blockedState.Tras % MAXTAM + 1 == blockedState.Frente)
        printf(" Erro   fila est  a  cheia\n");
    else {
        blockedState.vetor[blockedState.Tras - 1] = processo;
        blockedState.Tras = blockedState.Tras % MAXTAM + 1;
    }
}

void DesenfileiraReady(Processo *processo) {
    if (VaziaReady())
        printf("Erro fila esta vazia\n");
    else {
        *processo = readyState.vetor[readyState.Frente - 1];
        readyState.Frente = readyState.Frente % MAXTAM + 1;
    }
}

void DesenfileiraBlocked(Processo *processo) {
    if (VaziaBlocked())
        printf("Erro fila esta vazia\n");
    else {
        *processo = blockedState.vetor[blockedState.Frente - 1];
        blockedState.Frente = blockedState.Frente % MAXTAM + 1;
    }
}

void ImprimeReady() {
    int Aux;
    for (Aux = readyState.Frente - 1; Aux <= (readyState.Tras - 2); Aux++)
        printf("%12d\n", readyState.vetor[Aux]);
}

void ImprimeBlocked() {
    int Aux;
    for (Aux = blockedState.Frente - 1; Aux <= (blockedState.Tras - 2); Aux++)
        printf("%12d\n", blockedState.vetor[Aux]);
}

// Implementação Lista Arranjo

void FLVazia() {
    pcbTable.Primeiro = 0;
    pcbTable.Ultimo = pcbTable.Primeiro;
}


int Vazia() {
    return (pcbTable.Primeiro == pcbTable.Ultimo);
}

void Insere(Processo processo) {
    if (pcbTable.Ultimo > MAXTAM) printf("Lista esta cheia\n");
    else {
        pcbTable.vetor[pcbTable.Ultimo - 1] = processo;
        pcbTable.Ultimo++;
    }
}

void Retira(int indice, Processo *processo) {
    int Aux;

    if (Vazia() || indice >= pcbTable.Ultimo) {
        printf(" Erro   Posicao nao existe\n");
        return;
    }
    *processo = pcbTable.vetor[indice - 1];
    pcbTable.Ultimo--;
    for (Aux = indice; Aux < pcbTable.Ultimo; Aux++)
        pcbTable.vetor[Aux - 1] = pcbTable.vetor[Aux];
}

void Imprime() {
    int Aux;

    for (Aux = pcbTable.Primeiro - 1; Aux <= (pcbTable.Ultimo - 2); Aux++)
        printf("%d\n", pcbTable.vetor[Aux]);
}
