#include "ProcessCommander.h"
#include "ProcessManager.h"
#include "ProcessReporter.h"

void RunProcessCommander() {

    int fd[2]; /* File descriptors pro Pipe */
    pid_t pid_m, pid_r; /* Variável para armazenar o pid do fork do Processo Manager*/

    char ch = 0, str[2], str_enviada[BUFFER], instrucao[20];
    char str_recebida[BUFFER];
    FILE *arqCommander, *arqProgramaIni;

    int qtdeInstrucoes = 0;
    int opc = 0;
    int flag;

    Programa programa;
    FLVaziaPrograma(&programa);

    strcpy(str_enviada, "");

    do {
        printf("\nESCOLHA UMA OPÇÃO:\n\n1 - Entrada por Arquivo .TXT\n2 - Entrada Manual\n");
        printf("\n");
        scanf("%d",&opc);
        printf("\nESCOLHA UMA OPÇÃO:\n\n0 - Escalonamento Padrão.\n1 - Escalonamento por Loteria\n");
        printf("\n");
        scanf("%d",&tipoEscalonamento);
        printf("\n");
        if((opc != 1) && (opc != 2) && (tipoEscalonamento != 0) && (tipoEscalonamento != 1))
            printf("\nOpcão inválida!\n");
    } while((opc != 1) && (opc != 2) && (tipoEscalonamento != 0) && (tipoEscalonamento != 1));

    /* Criando Pipe. */
    if (pipe(fd) < 0) {
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
                        //strcat(str_enviada, str);
                        /* No pai, vamos ESCREVER, então vamos fechar a LEITURA do Pipe neste lado. */
                        close(fd[0]);

                        printf("String ENVIADA pelo COMMANDER de PID %i para MANAGER: '%s'\n", getpid(), str);

                        /* Escrevendo a string no pipe. */
                        write(fd[1], str, sizeof(str));
                        //strcpy(str_enviada, "");
                        sleep(3);
                    }
                }
            }

            fclose(arqCommander);


        } else{
            do{
                // sleep(2); // Pausa de 2 segundos para que não haja conflito retornos de saídas simultâneos
                printf("\n\n\nDigite a sequencia de comandos:\n\n");
                printf("Q: Executa instrução da lista.\n");
                printf("U: Desbloqueia processo.\n");
                printf("P: Estado atual do sistema.\n");
                printf("T: Tempo médio do ciclo.\n");
                scanf("%s",str);
                /* No pai, vamos ESCREVER, então vamos fechar a LEITURA do Pipe neste lado. */
                close(fd[0]);

                printf("\n\nString ENVIADA pelo COMMANDER de PID %i para MANAGER: '%s'\n", getpid(), str);

                /* Escrevendo a string no pipe. */
                write(fd[1], str, sizeof(str) + 1);

                sleep(2);

            }while (str[0] != 'T');

        }

        while (wait(NULL) != -1);
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

        InicializarEstruturas(&runningState, &readyState, &blockedState, &pcbTable, &cpu, &time);

        arqProgramaIni = fopen("ArquivoProgramaIni.txt", "r");

        if (arqProgramaIni == NULL) {
            printf("Erro, nao foi possivel abrir o arquivo ArquivoProgramaIni.txt\n");
        } else {
            while ((fgets(instrucao, sizeof(instrucao), arqProgramaIni)) != NULL)
                qtdeInstrucoes++;
        }

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

        Processo processo = CriarPrimeiroSimulado(&programa, &time, qtdeInstrucoes, getpid(), &ticketsSorteados);

        Enfileira(&readyState.filaPrioridade0, InserePcbTable(&pcbTable, processo));

        Escalonador(&cpu, &readyState, &runningState, &pcbTable, &ticketsSorteados);

        while (1){
            /* No filho, vamos ler. Então vamos fechar a entrada de ESCRITA do pipe. */
            close(fd[1]);

            /* Lendo o que foi escrito no pipe, e armazenando isso em 'str_recebida'. */
            read(fd[0], str_recebida, sizeof(str_recebida));

            printf("String LIDA pelo MANAGER de PID %i recebida pelo COMMANDER: '%s'\n\n", getpid(), str_recebida);

                printf("\nComando: %c\n", str_recebida[0]);
                switch (str_recebida[0]) {
                    case 'Q': /* Fim de uma unidade de tempo. Executa próxima instrução. */
                        flag = RunCPU(&cpu, &time, &pcbTable, &runningState, &blockedState, &readyState,
                                      &ticketsSorteados);
                        if (flag == 0) {
                            break;
                        }
                        break;
                    case 'U': /* Desbloqueia o primeiro processo simulado na fila bloqueada. */
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
                    case 'P': /* Imprime o estado atual do sistema. Dispara um novo processo reporter. */
                        /* Criando o processo Reporter. */
                        if ((pid_r = fork()) < 0) {
                            perror("fork");
                            exit(1);
                            /* Executando Reporter */
                        } else if (pid_r == 0) {
                            ImprimeReporter(&time, &pcbTable, &runningState, &blockedState, &readyState);
                            exit(0);
                        } else {
                            while (wait(NULL) != -1);
                        }
                        break;
                    case 'T': /* Imprime o tempo médio do ciclo e finaliza o sistema. */

                        /* Criando o processo Reporter. */
                        if ((pid_r = fork()) < 0) {
                            perror("fork");
                            exit(1);
                            /* Executando Reporter */
                        } else if (pid_r == 0) {
                            ImprimeReporter(&time, &pcbTable, &runningState, &blockedState, &readyState);
                            exit(0);
                        } else {
                            while (wait(NULL) != -1);
                        }

                        printf("\nTEMPO MEDIO DO CICLO: %f\n", CalcularTempoCiclo(&pcbTable));

                        exit(0);
                    default:
                        printf("Comando '%c' não aceito!", str_recebida[0]);
                        break;
            }
            strcpy(str_recebida, "");
            sleep(2);
        }
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

int PegarInstrucaoPrograma(Programa *programa, int indice, char *instrucao) {
    if (VaziaPrograma(programa) || indice >= programa->Ultimo) {
        printf("Erro Posicao nao existe Programa\n");
        return 0;
    }
    strcpy(instrucao, programa->instrucoes[indice].instrucao);
    return 1;
}