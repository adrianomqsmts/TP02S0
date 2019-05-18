#include "ProcessCommander.h"
#include "ProcessManager.h"
#include "ProcessReporter.h"

void runProcessCommander() {

    int fd1[2], opc = 0; /* File descriptors pro Pipe */
    pid_t pid_m, pid_r; /* Variável para armazenar o pid do fork do Processo Manager*/

    char ch, str[2], str_enviada[BUFFER], instrucao[20];
    char str_recebida[BUFFER];
    FILE *arqCommander, *arqProgramaIni;

    int qtdeInstrucoes = 0;
    int flag;

    Programa programa;
    FLVaziaPrograma(&programa);

    strcpy(str_enviada, "");

    do {
        printf("\nESCOLHA UMA OPÇÃO:\n\n1 - Entrada por Arquivo .TXT\n2 - Entrada Manual\n");
        printf("\n");
        scanf("%d",&opc);
        printf("\n");
        if((opc != 1) && (opc != 2))
            printf("\nOpcão inválida!\n");
    } while((opc != 1) && (opc != 2));

    /* Criando Pipe. */
    if (pipe(fd1) < 0) {
        perror("pipe");
        return;
    }

    /* Criando o processo filho. */
    if ((pid_m = fork()) < 0) {
        perror("fork");
        exit(1);
    }
    /* Processo Pai. */
    if (pid_m > 0) {

        if(opc == 1){

        arqCommander = fopen("ProcessCommander.txt", "r");

        if (arqCommander == NULL) {
            printf("Erro, nao foi possivel abrir o arquivo ProcessCommander.txt\n");
        } else {
            while ((ch = fgetc(arqCommander)) != EOF) {
                if (ch != ' ') {
                    str[0] = ch;
                    str[1] = '\0';
                    strcat(str_enviada, str);
                }
            }
        }

        fclose(arqCommander);
        } else {
            sleep(2); // Pausa de 2 segundos para que não haja conflito retornos de saídas simultâneos
            printf("\n\n\nDigite a sequência de comandos (ex: QQUQPT, QUUQP, etc):\n\n");
            printf("Q: Executa instrução da lista.\n");
            printf("U: Desbloqueia processo.\n");
            printf("P: Estado atual do sistema.\n");
            printf("T: Tempo médio do ciclo.\n\n");
            scanf("%s",str_enviada);
        }

        /* No pai, vamos ESCREVER, então vamos fechar a LEITURA do Pipe neste lado. */
        close(fd1[0]);

        printf("String ENVIADA pelo COMMANDER de PID %i para MANAGER: '%s'\n", getpid(), str_enviada);

        /* Escrevendo a string no pipe. */
        write(fd1[1], str_enviada, sizeof(str_enviada) + 1);
        exit(0);
    }
        /* Processo Filho (MANAGER). */
    else {

        RunningState runningState;
        ReadyState readyState;
        BlockedState blockedState;
        PcbTable pcbTable;
        Cpu cpu;
        Time time;
        Tickets ticketsSorteados;

        int indiceProcesso;

        inicializarEstruturas(&runningState, &readyState, &blockedState, &pcbTable, &cpu, &time);

        arqProgramaIni = fopen("ArquivoProgramaIni.txt", "r");

        if (arqProgramaIni == NULL) {
            printf("Erro, nao foi possivel abrir o arquivo ArquivoProgramaIni.txt\n");
        } else {
            while ((fgets(instrucao, sizeof(instrucao), arqProgramaIni)) != NULL)
                qtdeInstrucoes++;
        }

        printf("Quantidade de Instruções: %d\n", qtdeInstrucoes);

        fclose(arqProgramaIni);

        programa.tamanho = qtdeInstrucoes;

        arqProgramaIni = fopen("ArquivoProgramaIni.txt", "r");

        if (arqProgramaIni == NULL) {
            printf("Erro, nao foi possivel abrir o arquivo ArquivoProgramaIni.txt\n");
        } else {
            while ((fgets(instrucao, sizeof(instrucao), arqProgramaIni)) != NULL) {
                InserePrograma(&programa, instrucao);
            }
        }

        fclose(arqProgramaIni);

        Processo processo = criarPrimeiroSimulado(&programa, &time, qtdeInstrucoes, getpid(), &ticketsSorteados);

        Enfileira(&readyState.filaPrioridade0, InserePcbTable(&pcbTable, processo));

        /*ImprimeFila(&readyState.filaPrioridade0, &pcbTable);
        ImprimeFila(&readyState.filaPrioridade1, &pcbTable);
        ImprimeFila(&readyState.filaPrioridade2, &pcbTable);
        ImprimeFila(&readyState.filaPrioridade3, &pcbTable);*/

        /* No filho, vamos ler. Então vamos fechar a entrada de ESCRITA do pipe. */
        close(fd1[1]);

        /* Lendo o que foi escrito no pipe, e armazenando isso em 'str_recebida'. */
        read(fd1[0], str_recebida, sizeof(str_recebida));

        printf("String LIDA pelo MANAGER de PID %i recebida pelo COMMANDER: '%s'\n\n", getpid(), str_recebida);

        escalonador(&cpu, &readyState, &runningState, &pcbTable, &ticketsSorteados);

        for (int j = 0; j < strlen(str_recebida); j++) {
            printf("\nComando: %c\n", str_recebida[j]);
            switch (str_recebida[j]) {
                case 'Q': // Fim de uma unidade de tempo. Executa próxima instrução.
                    flag = runCPU(&cpu, &time, &pcbTable, &runningState, &blockedState, &readyState, &ticketsSorteados);
                    if (flag == 0) {
                        break;
                    }
                    break;
                case 'U': // Desbloqueia o primeiro processo simulado na fila bloqueada.
                    indiceProcesso = Desenfileira(&blockedState.filaBlockedState);
                    if (indiceProcesso != -1) {
                        processo = pcbTable.vetor[indiceProcesso];
                        strcpy(processo.estado, "PRONTO");
                        pcbTable.vetor[indiceProcesso] = processo;
                        printf("Processo de PID %i DESBLOQUEADO!\n", processo.pid);
                        switch (processo.prioridade) {
                            case 0:
                                Enfileira(&readyState.filaPrioridade0, indiceProcesso);
                                break;
                            case 1:
                                Enfileira(&readyState.filaPrioridade1, indiceProcesso);
                                break;
                            case 2:
                                Enfileira(&readyState.filaPrioridade2, indiceProcesso);
                                break;
                            case 3:
                                Enfileira(&readyState.filaPrioridade3, indiceProcesso);
                                break;
                        }
                    }
                    break;
                case 'P': // Imprime o estado atual do sistema. Dispara um novo processo reporter.
                    /* Criando o processo Reporter. */
                    if ((pid_r = fork()) < 0) {
                        perror("fork");
                        exit(1);
                        /* Executando Reporter */
                    } else if (pid_r == 0) {
                        imprimeReporter(&time, &pcbTable, &runningState, &blockedState, &readyState);
                        exit(0);
                    } else {
                        while (wait(NULL) != -1);
                    }
                    break;
                case 'T': // Imprime o tempo médio do ciclo e finaliza o sistema.

                    break;
                default:
                    printf("Comando '%c' não aceito!", str_recebida[j]);
                    break;
            }
        }
        printf("%s",str_enviada);
        //exit(0);

    }

}

void FLVaziaPrograma(Programa *programa) {
    programa->Primeiro = 0;
    programa->Ultimo = programa->Primeiro;
}


int VaziaPrograma(Programa *programa) {
    return (programa->Primeiro == programa->Ultimo);
}

int InserePrograma(Programa *programa, char *instrucao) {
    if (programa->Ultimo > MAXTAM) {
        printf("Lista esta cheia\n");
        return 0;
    } else {
        strcpy(programa->instrucoes[programa->Ultimo].instrucao, instrucao);
        programa->Ultimo++;
        return 1;
    }
}

int RetiraPrograma(Programa *programa, int indice, char *instrucao) {
    int Aux;

    if (VaziaPrograma(programa) || indice >= programa->Ultimo) {
        printf("Erro Posicao nao existe\n");
        return 0;
    }
    strcpy(instrucao, programa->instrucoes[indice].instrucao);
    programa->Ultimo--;
    for (Aux = indice; Aux < programa->Ultimo; Aux++)
        programa->instrucoes[Aux] = programa->instrucoes[Aux];
    return 1;
}

int PegarInstrucaoPrograma(Programa *programa, int indice, char *instrucao) {
    if (VaziaPrograma(programa) || indice >= programa->Ultimo) {
        printf("Erro Posicao nao existe Programa\n");
        return 0;
    }
    strcpy(instrucao, programa->instrucoes[indice].instrucao);
    return 1;
}
