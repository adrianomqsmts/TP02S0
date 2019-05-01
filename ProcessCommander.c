#include "ProcessCommander.h"
#include "ProcessReporter.h"
#include "ProcessManager.h"
#include "Process.h"

int runProcessCommander() {

    int fd[2], fd_s[2]; /* File descriptors pro Pipe */
    pid_t pid_m; /* Variável para armazenar o pid do fork do Processo Manager*/
    pid_t pid_s; /* Variável para armazenar o pid do fork do Primeiro Processo Simulado */

    char ch, str[2], str_enviada[BUFFER], instrucao[20];
    char str_recebida[BUFFER];
    FILE *arqCommander, *arqProgramaIni;

    int qtdeInstrucoes = 0, indiceUltimoProcesso = 0;

    TipoPrograma tipoPrograma;

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

        /* Criando Pipe. */
        if (pipe(fd_s) < 0) {
            perror("pipe");
            return -1;
        }

        inicializarEstruturas();

        /* No filho, vamos ler. Então vamos fechar a entrada de ESCRITA do pipe. */
        close(fd[1]);

        /* Lendo o que foi escrito no pipe, e armazenando isso em 'str_recebida'. */
        read(fd[0], str_recebida, sizeof(str_recebida));

        printf("String LIDA pelo MANAGER de PID %i recebida pelo COMMANDER: '%s'\n\n", getpid(), str_recebida);

        arqProgramaIni = fopen("ArquivoProgramaIni.txt", "r");

        if (arqProgramaIni == NULL) {
            printf("Erro, nao foi possivel abrir o arquivo ArquivoProgramaIni.txt\n");
        } else {
            int i = 0;
            while ((fgets(instrucao, sizeof(instrucao), arqProgramaIni)) != NULL) {
                strcpy(tipoPrograma.vetor[i].instrucao, instrucao);
                printf("%s", tipoPrograma.vetor[i].instrucao);
                i++;
                qtdeInstrucoes++;
            }
        }

        fclose(arqProgramaIni);

        for (int j = 0; j < strlen(str_recebida); j++) {
            printf("\n%c\n",str_recebida[j]);
            switch (str_recebida[j]) {
                case 'Q': // Fim de uma unidade de tempo. Executa próxima instrução.
                    /* No pai, vamos ESCREVER, então vamos fechar a LEITURA do Pipe neste lado. */
                    close(fd[0]);

                    printf("String ENVIADA pelo MANAGER de PID %i para SIMULADO: %s\n", getpid(), tipoPrograma.vetor[0].instrucao);
                    /* Escrevendo a string no pipe. */
                    write(fd[1], tipoPrograma.vetor[0].instrucao, sizeof(tipoPrograma.vetor[0].instrucao) + 1);
                    break;
                case 'U': // Desbloqueia o primeiro processo simulado na fila bloqueada.
                    ;
                    break;
                case 'P': // Imprime o estado atual do sistema.
                    ;
                    break;
                case 'T': // Imprime o tempo médio do ciclo e finaliza o sistema.
                    ;
                    break;
                default:
                    printf("Comando '%c' não aceito!", str_recebida[j]);
                    break;
            }
        }
        /* Criando o processo filho (SIMULADO). */
        if ((pid_s = fork()) < 0) {
            perror("fork");
            exit(1);
        }
        /* Processo filho (SIMULADO). */
        if (pid_s == 0) {

            indiceUltimoProcesso++;

            Processo processo;
            processo.pid = getpid();
            processo.pid_pai = getppid();
            strcpy(processo.estado, "PRONTO");
            processo.prioridade = 3;
            processo.estadoProcesso.inteiro = 0;
            processo.estadoProcesso.contador = 0;
            processo.estadoProcesso.tamanho = qtdeInstrucoes;
            for(int k = 0; k < 100; k++){
                strcpy(processo.estadoProcesso.programa[k].instrucao, tipoPrograma.vetor[k].instrucao);
            }

            pcbTable.vetor[indiceUltimoProcesso] = processo;

            /* No filho, vamos ler. Então vamos fechar a entrada de ESCRITA do pipe */
            close(fd[1]);

            /* Lendo o que foi escrito no pipe, e armazenando isso em 'str_recebida' */
            read(fd[0], str_recebida, sizeof(str_recebida));

            printf("String LIDA pelo SIMULADO de PID %i: '%s'\n\n", getpid(), str_recebida);

            executarInstrucao(&pcbTable.vetor[indiceUltimoProcesso].estadoProcesso, tipoPrograma.vetor[0].instrucao);
            exit(0);

        }

    }

}