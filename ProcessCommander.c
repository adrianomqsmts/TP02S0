#include "ProcessCommander.h"

int runProcessCommander() {

    int fd[2]; /* File descriptors pro Pipe */
    pid_t pid; /* Variável para armazenar o pid */

    char ch, str[2], mensagemCommander[100];
    FILE *arqCommander;

    strcpy(mensagemCommander, "");

    /* Criando Pipe */
    if (pipe(fd) < 0) {
        perror("pipe");
        return -1;
    }

    /* Criando o processo filho*/
    if ((pid = fork()) < 0) {
        perror("fork");
        exit(1);
    }
    /* Processo Pai*/
    if (pid > 0) {
        /*No pai, vamos ESCREVER, então vamos fechar a LEITURA do Pipe neste lado*/
        close(fd[0]);

        arqCommander = fopen("ProcessCommander.txt", "r");

        if (arqCommander == NULL) {
            printf("Erro, nao foi possivel abrir o arquivo ProcessCommander.txt\n");
        } else {
            while ((ch = fgetc(arqCommander)) != EOF) {
                if (ch != ' ') {
                    str[0] = ch;
                    str[1] = '\0';
                    strcat(mensagemCommander, str);
                }
            }
        }

        fclose(arqCommander);

        printf("String enviada pelo Commander de PID %i no Pipe: '%s'\n", getpid(), mensagemCommander);

        /* Escrevendo a string no pipe */
        write(fd[1], mensagemCommander, sizeof(mensagemCommander) + 1);
        exit(0);
    }
    /* Processo Filho*/
    else {
        char str_recebida[BUFFER];

        /* No filho, vamos ler. Então vamos fechar a entrada de ESCRITA do pipe */
        close(fd[1]);

        /* Lendo o que foi escrito no pipe, e armazenando isso em 'str_recebida' */
        read(fd[0], str_recebida, sizeof(str_recebida));

        printf("String lida pelo Manager de PID %i no Pipe : '%s'\n\n", getpid(), str_recebida);

        for(int i = 0; i < strlen(str_recebida);i++){
            switch(str_recebida[i]){
                case 'Q': //  Fim de uma unidade de tempo.
                    ; break;
                case 'U': //  Desbloqueia o primeiro processo simulado na fila bloqueada.
                    ; break;
                case 'P': // Imprime o estado atual do sistema.
                    ; break;
                case 'T': // Imprime o tempo médio do ciclo e finaliza o sistema.
                    ; break;
                default:
                    printf("Comando '%c' não aceito!", str_recebida[i]);  break;
            }
        }
        exit(0);
    }

}