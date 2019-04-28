#include "Process.h"

void alocarPrograma(EstadoProcesso *estadoProcesso) {
    estadoProcesso->programa = (char *) malloc(estadoProcesso->tamanho * sizeof(char));
}

void liberarPrograma(EstadoProcesso *estadoProcesso) {
    free(estadoProcesso->programa);
}

void executarInstrucoes(EstadoProcesso *estadoProcesso){
    char instrucao;
    int n;

    switch (instrucao){
        case 'S':
            estadoProcesso->inteiro = n; break;
        case 'A':
            estadoProcesso->inteiro += n; break;
        case 'D':
            estadoProcesso->inteiro -= n; break;
        case 'B':
            ;
        case 'E':
            ;
        case 'F':
            ;
        case 'R':
            ;
    }

}