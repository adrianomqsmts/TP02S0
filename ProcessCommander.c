#include "ProcessCommander.h"
#include "ProcessManager.h"

int runProcessCommander() {

    int fd[2], fd_s[2]; /* File descriptors pro Pipe */
    pid_t pid_m; /* Variável para armazenar o pid do fork do Processo Manager*/

    char ch, str[2], str_enviada[BUFFER], instrucao[20];
    char str_recebida[BUFFER];
    FILE *arqCommander, *arqProgramaIni;

    int qtdeInstrucoes = 0;
    int qtdeComandos = 0;

    Programa programa;
    FFVazia(&programa);

    strcpy(str_enviada, "");

    /* Criando Pipe. */
    if (pipe(fd) < 0) {
        perror("pipe");
        return -1;
    }

    /* Criando o processo filho. */
    if ((pid_m = fork()) < 0) {
        perror("fork");
        exit(1);
    }
    /* Processo Pai. */
    if (pid_m > 0) {

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

        /* No pai, vamos ESCREVER, então vamos fechar a LEITURA do Pipe neste lado. */
        close(fd[0]);

        printf("String ENVIADA pelo COMMANDER de PID %i para MANAGER: '%s'\n", getpid(), str_enviada);

        /* Escrevendo a string no pipe. */
        write(fd[1], str_enviada, sizeof(str_enviada) + 1);
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

        Processo processoDesbloqueado;
        int desenfileirou;
        int pidProximoSimulado = 0;

        /* Criando Pipe. */
        if (pipe(fd_s) < 0) {
            perror("pipe");
            return -1;
        }

        inicializarEstruturas(&runningState, &readyState, &blockedState, &pcbTable, &cpu, &time);

        arqProgramaIni = fopen("ArquivoProgramaIni.txt", "r");

        if (arqProgramaIni == NULL) {
            printf("Erro, nao foi possivel abrir o arquivo ArquivoProgramaIni.txt\n");
        } else {
            while( (fgets(instrucao, sizeof(instrucao), arqProgramaIni)) != NULL )
                    qtdeInstrucoes++;
        }

        printf("Quantidade de Instruções: %d\n", qtdeInstrucoes);

        fclose(arqProgramaIni);

        programa.tamanho = qtdeInstrucoes;
        //alocarPrograma(&programa);

        arqProgramaIni = fopen("ArquivoProgramaIni.txt", "r");

        if (arqProgramaIni == NULL) {
            printf("Erro, nao foi possivel abrir o arquivo ArquivoProgramaIni.txt\n");
        } else {
            while ((fgets(instrucao, sizeof(instrucao), arqProgramaIni)) != NULL) {
                EnfileiraPrograma(&programa, instrucao);
            }
        }

        fclose(arqProgramaIni);

        Processo processo = criarPrimeiroSimulado(&programa, &time, qtdeInstrucoes, getpid());

        EnfileiraReady(&readyState, &processo);

        ImprimeReady(&readyState);

        InserePcbTable(&pcbTable, processo);

        /* No filho, vamos ler. Então vamos fechar a entrada de ESCRITA do pipe. */
        close(fd[1]);

        /* Lendo o que foi escrito no pipe, e armazenando isso em 'str_recebida'. */
        read(fd[0], str_recebida, sizeof(str_recebida));

        printf("String LIDA pelo MANAGER de PID %i recebida pelo COMMANDER: '%s'\n\n", getpid(), str_recebida);

        processo = colocarProcessoCPU(&cpu, &readyState);

        for (int j = 0; j < strlen(str_recebida); j++) {
            //printf("\n%c\n", str_recebida[j]);
            switch (str_recebida[j]) {
                case 'Q': // Fim de uma unidade de tempo. Executa próxima instrução.
                    runCPU(&cpu, &time, &pcbTable, &runningState, &blockedState, &readyState, &processo);
                    ImprimePcbTable(&pcbTable);
                    ImprimirCPU(&cpu);
                    break;
                case 'U': // Desbloqueia o primeiro processo simulado na fila bloqueada.
                    desenfileirou = DesenfileiraBlocked(&blockedState, &processoDesbloqueado);
                    if(desenfileirou)
                        EnfileiraReady(&readyState, &processoDesbloqueado);
                    break;
                case 'P': // Imprime o estado atual do sistema. Dispara um novo processo reporter.

                    break;
                case 'T': // Imprime o tempo médio do ciclo e finaliza o sistema.

                    break;
                default:
                    printf("Comando '%c' não aceito!", str_recebida[j]);
                    break;
            }
        }


        //exit(0);

    }

}

/*void alocarPrograma(Programa *programa) {
    programa->instrucoes = (Instrucao *) malloc(programa->tamanho * sizeof(Instrucao));
}

void liberarPrograma(Programa *programa) {
    free(programa->instrucoes);
}

void alocarEstadoPrograma(EstadoProcesso *estadoProcesso) {
    estadoProcesso->instrucoes = (Instrucao *) malloc(estadoProcesso->tamanho * sizeof(Instrucao));
}

void liberarEstadoPrograma(EstadoProcesso *estadoProcesso) {
    free(estadoProcesso->instrucoes);
}*/

void FFVazia(Programa *programa) {
    programa->Frente = 0;
    programa->Tras = programa->Frente;
}

int EhVazia(Programa *programa) { return (programa->Frente == programa->Tras); }

void EnfileiraPrograma(Programa *programa, char *instrucao) {
    if (programa->Tras % MAXTAM + 1 == programa->Frente)
        printf("Erro fila esta cheia\n");
    else {
        strcpy(programa->instrucoes[programa->Tras].instrucao, instrucao);
        programa->Tras = programa->Tras % MAXTAM + 1;
    }
}

void DesenfileiraPrograma(Programa *programa, char *instrucao) {
    if (EhVazia(programa))
        printf("Erro fila esta vazia\n");
    else {
        strcpy(instrucao, programa->instrucoes[programa->Frente].instrucao);
        programa->Frente = programa->Frente % MAXTAM + 1;
    }
}