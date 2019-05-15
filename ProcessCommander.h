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

void runProcessCommander();
void FLVaziaPrograma(Programa *programa);
int VaziaPrograma(Programa *programa);
int InserePrograma(Programa *programa, char *instrucao);
int RetiraPrograma(Programa *programa, int indice, char *instrucao);
int PegarInstrucaoPrograma(Programa *programa, int indice, char *instrucao);

#endif //TP02SO_PROCESSCOMMANDER_H
