
#ifndef TP02SO_ESTRUTURASCOMPARTILHADAS_H
#define TP02SO_ESTRUTURASCOMPARTILHADAS_H

typedef struct Instrucao {
    char instrucao[20];
} Instrucao;

typedef struct Programa {
    Instrucao instrucoes[20];
    int tamanho;
    int Frente, Tras;
} Programa;

typedef struct EstadoProcesso{
    int inteiro;
    int contador; // Inicializar com 0
    int tamanho;
    Instrucao programa[20]; // Armazenar as instruções com cada elemento do instrucoes contendo uma instrução..
} EstadoProcesso;

#endif //TP02SO_ESTRUTURASCOMPARTILHADAS_H
