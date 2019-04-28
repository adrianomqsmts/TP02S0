/*
#include "ProcessManager.h"

void runProcessManager(){

}

// Implementação Fila Arranjo

void FFVaziaReady(READY_STATE *readyState) {
    readyState->Frente = 1;
    readyState->Tras = readyState->Frente;
}

void FFVaziaBlocked(BLOCKED_STATE *blockedState) {
    blockedState->Frente = 1;
    blockedState->Tras = blockedState->Frente;
}

int VaziaReady(READY_STATE readyState) { return (readyState.Frente == readyState.Tras); }

int VaziaBlocked(BLOCKED_STATE blockedState) { return (blockedState.Frente == blockedState.Tras); }

void EnfileiraReady(TipoItem x, READY_STATE *readyState) {
    if (readyState->Tras % MAXTAM + 1 == readyState->Frente)
        printf(" Erro fila esta cheia\n");
    else {
        readyState->vetor[readyState->Tras - 1] = x;
        readyState->Tras = readyState->Tras % MAXTAM + 1;
    }
}

void EnfileiraBlocked(TipoItem x, BLOCKED_STATE *blockedState) {
    if (blockedState->Tras % MAXTAM + 1 == blockedState->Frente)
        printf(" Erro   fila est  a  cheia\n");
    else {
        blockedState->vetor[blockedState->Tras - 1] = x;
        blockedState->Tras = blockedState->Tras % MAXTAM + 1;
    }
}

void DesenfileiraReady(READY_STATE *readyState, TipoItem *Item) {
    if (VaziaReady(*readyState))
        printf("Erro fila esta vazia\n");
    else {
        *Item = readyState->vetor[readyState->Frente - 1];
        readyState->Frente = readyState->Frente % MAXTAM + 1;
    }
}

void DesenfileiraBlocked(BLOCKED_STATE *blockedState, TipoItem *Item) {
    if (VaziaBlocked(*blockedState))
        printf("Erro fila esta vazia\n");
    else {
        *Item = blockedState->vetor[blockedState->Frente - 1];
        blockedState->Frente = blockedState->Frente % MAXTAM + 1;
    }
}

void ImprimeReady(READY_STATE readyState) {
    int Aux;
    for (Aux = readyState.Frente - 1; Aux <= (readyState.Tras - 2); Aux++)
        printf("%12d\n", readyState.vetor[Aux]);
}

void ImprimeBlocked(BLOCKED_STATE blockedState) {
    int Aux;
    for (Aux = blockedState.Frente - 1; Aux <= (blockedState.Tras - 2); Aux++)
        printf("%12d\n", blockedState.vetor[Aux]);
}

// Implementação Lista Arranjo

void FLVazia(PC_TABLE *pcTable) {
    pcTable->Primeiro = 0;
    pcTable->Ultimo = pcTable->Primeiro;
}


int Vazia(PC_TABLE pcTable) {
    return (pcTable.Primeiro == pcTable.Ultimo);
}


void Insere(TipoItem x, PC_TABLE *pcTable) {
    if (pcTable->Ultimo > MAXTAM) printf("Lista esta cheia\n");
    else {
        pcTable->vetor[pcTable->Ultimo - 1] = x;
        pcTable->Ultimo++;
    }
}


void Retira(int indice, PC_TABLE *pcTable, TipoItem *Item) {
    int Aux;

    if (Vazia(*pcTable) || indice >= pcTable->Ultimo) {
        printf(" Erro   Posicao nao existe\n");
        return;
    }
    *Item = pcTable->vetor[indice - 1];
    pcTable->Ultimo--;
    for (Aux = indice; Aux < pcTable->Ultimo; Aux++)
        pcTable->vetor[Aux - 1] = pcTable->vetor[Aux];
}


void Imprime(PC_TABLE *pcTable) {
    int Aux;

    for (Aux = pcTable->Primeiro - 1; Aux <= (pcTable->Ultimo - 2); Aux++)
        printf("%d\n", pcTable->vetor[Aux]);
}
*/
