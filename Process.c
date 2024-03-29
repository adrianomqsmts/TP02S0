#include "Process.h"

int ExecutarInstrucao(Cpu *cpu, Time *time, RunningState *runningState, PcbTable *pcbTable, BlockedState *blockedState,
                      ReadyState *readyState, Processo *processo, Tickets *ticketsSorteados) {
    char comando, instrucao[20], nomeArquivo[20];
    FILE *arqPrograma;
    int n = 0, qtdeInstrucoes = 0, j = 0, flag;
    Processo novoProcesso, processoRetirado;
    Programa novoPrograma;
    FLVaziaPrograma(&novoPrograma);

    strcpy(instrucao, "");
    strcpy(nomeArquivo, "");

    flag = PegarInstrucaoPrograma(&cpu->programa, cpu->contadorProgramaAtual, instrucao);

    if (flag == 0) {
        printf("\nAcabaram as instrucoes do processo de PID %i.\n", processo->pid);
        printf("Encerrando processo e colocando outro na CPU...\n");
        RetiraPcbTable(pcbTable, runningState->iPcbTable, &processoRetirado);
        switch (processoRetirado.prioridade) {
            case 0:
                AtualizaFila(&readyState->filaPrioridade0, runningState->iPcbTable); break;
            case 1:
                AtualizaFila(&readyState->filaPrioridade1, runningState->iPcbTable); break;
            case 2:
                AtualizaFila(&readyState->filaPrioridade2, runningState->iPcbTable); break;
            case 3:
                AtualizaFila(&readyState->filaPrioridade3, runningState->iPcbTable); break;
        }
        time->time++;
        return 0;
    }

    char *p = instrucao;
    while (*p) { // While there are more characters to process...
        if (isdigit(*p) || ((*p == '-' || *p == '+') && isdigit(*(p + 1)))) {
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

    switch (comando) {
        case 'S':  /* Define o valor da variável inteira para n, onde n é um inteiro. */
            cpu->valorInteiro = n;
            printf("Variavel inteira: %d\n", cpu->valorInteiro);
            cpu->contadorProgramaAtual++;
            cpu->quantum++;
            time->time++;
            return 1;
        case 'A': /* Adiciona n ao valor da variável inteira, onde n é um inteiro. */
            cpu->valorInteiro += n;
            printf("Variavel inteira: %d\n", cpu->valorInteiro);
            cpu->contadorProgramaAtual++;
            cpu->quantum++;
            time->time++;
            return 1;
        case 'D': /* Subtrai n do valor da variável inteira, onde n é um inteiro. */
            cpu->valorInteiro -= n;
            printf("Variavel inteira: %d\n", cpu->valorInteiro);
            cpu->contadorProgramaAtual++;
            cpu->quantum++;
            time->time++;
            return 1;
        case 'B': /* Bloqueia esse processo simulado. */
            strcpy(pcbTable->vetor[runningState->iPcbTable].estado, "BLOQUEADO");
            if(pcbTable->vetor[runningState->iPcbTable].prioridade > 0)
                pcbTable->vetor[runningState->iPcbTable].prioridade--;
            Enfileira(&blockedState->filaBlockedState, runningState->iPcbTable);
            printf("Processo de PID %i adicionado a Fila de Bloqueados!\n", pcbTable->vetor[runningState->iPcbTable].pid);
            cpu->contadorProgramaAtual++;
            cpu->quantum++;
            time->time++;
            return 0;
        case 'E': /* Termina esse processo simulado. */
            printf("Posicao do processo de PID %i encerrado: %d\n", processo->pid, runningState->iPcbTable);
            RetiraPcbTable(pcbTable, runningState->iPcbTable, &processoRetirado);
            pcbTable->tempoCPUEncerrados += processoRetirado.tempoCPU;
            switch (processoRetirado.prioridade) {
                case 0:
                    AtualizaFila(&readyState->filaPrioridade0, runningState->iPcbTable); break;
                case 1:
                    AtualizaFila(&readyState->filaPrioridade1, runningState->iPcbTable); break;
                case 2:
                    AtualizaFila(&readyState->filaPrioridade2, runningState->iPcbTable); break;
                case 3:
                    AtualizaFila(&readyState->filaPrioridade3, runningState->iPcbTable); break;
            }
            cpu->quantum++;
            time->time++;
            return 0;
        case 'F': /* Cria um novo processo simulado. */
            processo->estadoProcesso.contador = cpu->contadorProgramaAtual;
            novoProcesso = CriarProcessoSimulado(time, processo, ticketsSorteados);
            switch (novoProcesso.prioridade) {
                case 0:
                    Enfileira(&readyState->filaPrioridade0, InserePcbTable(pcbTable, novoProcesso)); break;
                case 1:
                    Enfileira(&readyState->filaPrioridade1, InserePcbTable(pcbTable, novoProcesso)); break;
                case 2:
                    Enfileira(&readyState->filaPrioridade2, InserePcbTable(pcbTable, novoProcesso)); break;
                case 3:
                    Enfileira(&readyState->filaPrioridade3, InserePcbTable(pcbTable, novoProcesso)); break;
            }
            cpu->contadorProgramaAtual += n + 1; /* Necessário para atualizar o contador do processo pai para a
                                                  * instrução logo após a instrução F. */
            cpu->quantum++;
            time->time++;
            return 1;
        case 'R': /* Substitui o programa do processo simulado pelo programa no arquivo nome_do_arquivo e define o
                   * contador de programa para a primeira instrução desse novo programa. */
            for (int i = 2; i < strlen(instrucao); i++) {
                if (instrucao[i] != 10) {
                    nomeArquivo[j] = instrucao[i];
                    j++;
                } else {
                    nomeArquivo[j - 1] = '\0';
                    j++;
                }

            }

            printf("Nome Arquivo: %s\n", nomeArquivo);

            arqPrograma = fopen(nomeArquivo, "r");

            if (arqPrograma == NULL) {
                printf("Erro, nao foi possivel abrir o arquivo %s\n", nomeArquivo);
            } else {
                while ((fgets(instrucao, sizeof(instrucao), arqPrograma)) != NULL) {
                    InserePrograma(&novoPrograma, instrucao);
                    qtdeInstrucoes++;
                }
                novoPrograma.tamanho = qtdeInstrucoes;
                cpu->programa = novoPrograma;
                cpu->contadorProgramaAtual = 0;
                cpu->valorInteiro = 0;
                cpu->quantum++;
                time->time++;

            }
            fclose(arqPrograma);

            return 1;
        default:
            printf("Comando não suportado!\n");
            return 2;
    }

}