#ifndef TP02SO_PROCESSCOMMANDER_H
#define TP02SO_PROCESSCOMMANDER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "Process.h"

#define BUFFER 256

typedef struct TipoPrograma {
    TipoInstrucao vetor[100];
} TipoPrograma;

int runProcessCommander();

#endif //TP02SO_PROCESSCOMMANDER_H
