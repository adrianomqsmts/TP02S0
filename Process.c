#include "Process.h"
#include "EstruturasCompartilhadas.h"

void executarInstrucao(Cpu *cpu, Time *time, RunningState *runningState, PcbTable *pcbTable, BlockedState *blockedState, ReadyState *readyState, Processo *processo){
    char ch, comando, instrucao[20];
    FILE *arqPrograma;
    int n = 0;

    strcpy(instrucao, "");

    DesenfileiraPrograma(&cpu->programa, instrucao);

    char *p = instrucao;
    while (*p) { // While there are more characters to process...
        if ( isdigit(*p) || ( (*p=='-'||*p=='+') && isdigit(*(p+1)) )) {
            // Found a number
            n = strtol(p, &p, 10); // Read number
            printf("Inteiro extraido: %d\n", n); // and print it.
        } else {
            // Otherwise, move on to the next character.
            p++;
        }
    }

    comando = instrucao[0];

    printf("Comando: %c\n", comando);

    switch (comando){
        case 'S':  /* Define o valor da variável inteira para n, onde n é um inteiro. */
            cpu->valorInteiro = n;
            printf("Variavel inteira: %d\n", cpu->valorInteiro);
            cpu->contadorProgramaAtual++;
            time->time++;
            break;
        case 'A': /* Adiciona n ao valor da variável inteira, onde n é um inteiro. */
            cpu->valorInteiro += n;
            printf("Variavel inteira: %d\n", cpu->valorInteiro);
            cpu->contadorProgramaAtual++;
            time->time++;
            break;
        case 'D': /* Subtrai n do valor da variável inteira, onde n é um inteiro. */
            cpu->valorInteiro -= n;
            printf("Variavel inteira: %d\n", cpu->valorInteiro);
            cpu->contadorProgramaAtual++;
            time->time++;
            break;
        case 'B': /* Bloqueia esse processo simulado. */
            EnfileiraBlocked(blockedState, processo);
            cpu->contadorProgramaAtual++;
            time->time++;
            break;
        case 'E': /* Termina esse processo simulado. */
            cpu->contadorProgramaAtual++;
            time->time++;
            break;
        case 'F': /* Cria um novo processo simulado. */
            time->time++;
            break;
        case 'R': /* Substitui o instrucoes do processo simulado pelo instrucoes no arquivo nome_do_arquivo. */
            arqPrograma = fopen("ArquivoPrograma.txt", "r");

            if (arqPrograma == NULL) {
                printf("Erro, nao foi possivel abrir o arquivo ArquivoPrograma.txt\n");
            } else {
                while ((ch = fgetc(arqPrograma)) != EOF) {
                    if (ch != ' ') {

                    }
                }
            }

            fclose(arqPrograma);

            time->time++;

            break;
        default:
            printf("Comando não suportado!\n");
    }

}