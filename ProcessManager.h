#ifndef TP02SO_PROCESSMANAGER_H
#define TP02SO_PROCESSMANAGER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "ProcessCommander.h"


#define MAXTAM 100
#define BUFFER 256

typedef struct Time {
    int time;
} Time;

typedef struct Cpu {
    Programa programa;
    int contadorProgramaAtual;
    int valorInteiro;
    int fatiaTempo;
    int fatiaTempoUsada;
    int quantum;
} Cpu;

typedef struct Tickets {
    int vetor[MAXTAM];
    int Primeiro, Ultimo;
} Tickets;

typedef struct Fila {
    char nome[30];
    int vetor[MAXTAM];
    int Frente, Tras;
} Fila;

typedef struct EstadoProcesso{
    int inteiro;
    int contador;
    int tamanho;
    Programa programa;
} EstadoProcesso;

typedef struct Processo{
    pid_t pid;
    pid_t pid_pai;
    int prioridade;
    char estado[15];
    int timeInicio;
    int tempoCPU;
    Tickets tickets;
    EstadoProcesso estadoProcesso;
} Processo;

typedef struct PcbTable {
    Processo vetor[MAXTAM];
    float tempoCPUEncerrados;
    int Primeiro, Ultimo;
} PcbTable;

typedef struct ReadyState {
    Fila filaPrioridade0;
    Fila filaPrioridade1;
    Fila filaPrioridade2;
    Fila filaPrioridade3;
} ReadyState;

typedef struct BlockedState {
    Fila filaBlockedState;
} BlockedState;

typedef struct RunningState {
    int iPcbTable;
} RunningState;

int tipoEscalonamento; /* Se != 0 política do grupo, se == 0 política do professor. */

void InicializarEstruturas(RunningState *runningState, ReadyState *readyState, BlockedState *blockedState,
                           PcbTable *pcbTable, Cpu *cpu, Time *time);
Processo CriarPrimeiroSimulado(Programa *programa, Time *timee, int qtdeInstrucoes, int pidPai,
                               Tickets *ticketsSorteados);
Processo CriarProcessoSimulado(Time *timee, Processo *processoPai, Tickets *ticketsSorteados);
int Escalonador(Cpu *cpu, ReadyState *readyState, RunningState *runningState, PcbTable *pcbTable,
                Tickets *ticketsSorteados);
void ImprimirCPU(Cpu *cpu);
int RunCPU(Cpu *cpu, Time *time, PcbTable *pcbTable, RunningState *runningState, BlockedState *blockedState,
           ReadyState *readyState, Tickets *ticketsSorteados);
float CalcularTempoCiclo(PcbTable *pcbTable);

void FFVazia(Fila *fila);
int EhVazia(Fila *fila);
void Enfileira(Fila *fila, int indiceProcesso);
int Desenfileira(Fila *fila);
void ImprimeFila(Fila *fila, PcbTable *pcbTable);
void AtualizaFila(Fila *fila, int indiceProcesso);
void RemoverProcessoFila(Fila *fila, int indiceProcesso);
int ProcessoJaInserido(Fila *fila, int indiceProcesso);

void FLVaziaPcbTable(PcbTable *pcbTable);
int FEhVaziaPcbTable(PcbTable *pcbTable);
int InserePcbTable(PcbTable *pcbTable, Processo processo);
int RetiraPcbTable(PcbTable *pcbTable, int indice, Processo *processo);
void ImprimePcbTable(PcbTable *pcbTable);

void FLVaziaTickets(Tickets *tickets);
int FEhVaziaTickets(Tickets *tickets);
int InsereTickets(Tickets *tickets, int ticket);
int PegaTicket(Tickets *tickets);
int TicketExiste(Tickets *tickets, int ticket);
void ImprimeTickets(Tickets *tickets);


#endif //TP02SO_PROCESSMANAGER_H
