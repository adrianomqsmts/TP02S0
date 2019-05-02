#ifndef TP02SO_PROCESSCOMMANDER_H
#define TP02SO_PROCESSCOMMANDER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "EstruturasCompartilhadas.h"

#define MAXTAM 100
#define BUFFER 256

int runProcessCommander();
void alocarPrograma(Programa *programa);
void liberarPrograma(Programa *programa);
void alocarEstadoPrograma(EstadoProcesso *estadoProcesso);
void liberarEstadoPrograma(EstadoProcesso *estadoProcesso);
void FFVazia(Programa *programa);
int EhVazia(Programa *programa);
void Enfileira(Programa *programa, char instrucao[]);
void Desenfileira(Programa *programa, char instrucao[20]);

#endif //TP02SO_PROCESSCOMMANDER_H
