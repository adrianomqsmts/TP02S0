#include "Process.h"
#include "ProcessManager.h"

/*void alocarPrograma(EstadoProcesso *estadoProcesso) {
    estadoProcesso->programa = (char *) malloc(estadoProcesso->tamanho * sizeof(char));
}*/

void liberarPrograma(EstadoProcesso *estadoProcesso) {
    free(estadoProcesso->programa);
}

void executarInstrucao(EstadoProcesso *estadoProcesso, char instrucao[]){
    char ch, comando;
    FILE *arqPrograma;
    int n;

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
            estadoProcesso->inteiro = n;
            estadoProcesso->contador++;
            printf("Variavel inteira: %d\n", estadoProcesso->inteiro);
            break;
        case 'A': /* Adiciona n ao valor da variável inteira, onde n é um inteiro. */
            estadoProcesso->inteiro += n;
            estadoProcesso->contador++;
            printf("Variavel inteira: %d\n", estadoProcesso->inteiro);
            break;
        case 'D': /* Subtrai n do valor da variável inteira, onde n é um inteiro.  */
            estadoProcesso->inteiro -= n;
            estadoProcesso->contador++;
            printf("Variavel inteira: %d\n", estadoProcesso->inteiro);
            break;
        case 'B': /* Bloqueia esse processo simulado.   */
            estadoProcesso->contador++;
            break;
        case 'E': /* Termina esse processo simulado.    */
            estadoProcesso->contador++;
            break;
        case 'F': /* Cria um novo processo simulado.    */
            estadoProcesso->contador++;
            break;
        case 'R': /* Substitui o programa do processo simulado pelo programa no arquivo nome_do_arquivo.   */
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

            estadoProcesso->contador++;
            break;
        default:
            printf("Comando não suportado!\n");
    }

}