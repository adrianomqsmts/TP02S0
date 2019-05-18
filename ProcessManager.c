#include "ProcessManager.h"
#include "Process.h"

int pidNovoProcesso = 0;
int tipoEscalonamento = 1; /* Se != 0 política do grupo, se == 0 política do professor. */

void inicializarEstruturas(RunningState *runningState, ReadyState *readyState, BlockedState *blockedState,
                           PcbTable *pcbTable, Cpu *cpu, Time *time) {
    runningState->iPcbTable = -1;

    FFVazia(&readyState->filaPrioridade0);
    strcpy(readyState->filaPrioridade0.nome, "FILA DE PRIORIDADE 0");
    FFVazia(&readyState->filaPrioridade1);
    strcpy(readyState->filaPrioridade1.nome, "FILA DE PRIORIDADE 1");
    FFVazia(&readyState->filaPrioridade2);
    strcpy(readyState->filaPrioridade2.nome, "FILA DE PRIORIDADE 2");
    FFVazia(&readyState->filaPrioridade3);
    strcpy(readyState->filaPrioridade3.nome, "FILA DE PRIORIDADE 3");

    FFVazia(&blockedState->filaBlockedState);
    strcpy(blockedState->filaBlockedState.nome, "FILA BLOCKED");

    FLVaziaPcbTable(pcbTable);

    FLVaziaPrograma(&cpu->programa);
    cpu->fatiaTempoUsada = 0;
    cpu->fatiaTempo = 0;
    cpu->contadorProgramaAtual = 0;
    cpu->valorInteiro = 0;

    time->time = 0;
}

Processo
criarPrimeiroSimulado(Programa *programa, Time *timee, int qtdeInstrucoes, int pidPai, Tickets *ticketsSorteados) {

    srand(time(NULL));
    Processo processo;

    FLVaziaTickets(&processo.tickets);

    processo.pid = pidNovoProcesso;
    processo.pid_pai = pidPai;
    processo.prioridade = 0;

    while (processo.tickets.Ultimo < 5) {
        int ticket = rand() % 100;
        if (!TicketExiste(ticketsSorteados, ticket)) {
            InsereTickets(&processo.tickets, ticket);
            InsereTickets(ticketsSorteados, ticket);
        }
    }

    //ImprimeTickets(&processo.tickets);
    //ImprimeTickets(ticketsSorteados);

    processo.tempoCPU = 0;
    processo.timeInicio = timee->time;
    processo.estadoProcesso.inteiro = 0;
    processo.estadoProcesso.contador = 0;
    processo.estadoProcesso.tamanho = qtdeInstrucoes;
    processo.estadoProcesso.programa = *programa;
    strcpy(processo.estado, "PRONTO");
    pidNovoProcesso++;
    return processo;
}

Processo criarProcessoSimulado(Time *timee, Processo *processoPai, Tickets *ticketsSorteados) {

    srand(time(NULL));
    Processo processo;

    FLVaziaTickets(&processo.tickets);

    processo.pid = pidNovoProcesso;
    processo.pid_pai = processoPai->pid;
    processo.prioridade = processoPai->prioridade;

    while (processo.tickets.Ultimo < 5) {
        int ticket = rand() % 100;
        if (!TicketExiste(ticketsSorteados, ticket)) {
            InsereTickets(&processo.tickets, ticket);
            InsereTickets(ticketsSorteados, ticket);
        }
    }

    //ImprimeTickets(&processo.tickets);
    //ImprimeTickets(ticketsSorteados);

    processo.tempoCPU = 0;
    processo.timeInicio = timee->time;
    processo.estadoProcesso.inteiro = processoPai->estadoProcesso.inteiro;
    processo.estadoProcesso.contador = processoPai->estadoProcesso.contador + 1;
    processo.estadoProcesso.tamanho = processoPai->estadoProcesso.tamanho;
    processo.estadoProcesso.programa = processoPai->estadoProcesso.programa;
    strcpy(processo.estado, "PRONTO");
    pidNovoProcesso++;
    return processo;
}

int escalonador(Cpu *cpu, ReadyState *readyState, RunningState *runningState, PcbTable *pcbTable,
                Tickets *ticketsSorteados) {

    Processo processo;
    int ticket;
    int pegou = 0;

    if (tipoEscalonamento == 0) {
        if (!EhVazia(&readyState->filaPrioridade0)) {
            runningState->iPcbTable = Desenfileira(&readyState->filaPrioridade0);
        } else if ((!EhVazia(&readyState->filaPrioridade1)) && EhVazia(&readyState->filaPrioridade0)) {
            runningState->iPcbTable = Desenfileira(&readyState->filaPrioridade1);
        } else if ((!EhVazia(&readyState->filaPrioridade2)) && EhVazia(&readyState->filaPrioridade1) &&
                   EhVazia(&readyState->filaPrioridade0)) {
            runningState->iPcbTable = Desenfileira(&readyState->filaPrioridade2);
        } else if ((!EhVazia(&readyState->filaPrioridade3)) && EhVazia(&readyState->filaPrioridade2) &&
                   EhVazia(&readyState->filaPrioridade1) && EhVazia(&readyState->filaPrioridade0)) {
            runningState->iPcbTable = Desenfileira(&readyState->filaPrioridade3);
        } else {
            printf("Não existe processos nas filas de prioridades!\n");
            ImprimeFila(&readyState->filaPrioridade0, pcbTable);
            ImprimeFila(&readyState->filaPrioridade1, pcbTable);
            ImprimeFila(&readyState->filaPrioridade2, pcbTable);
            ImprimeFila(&readyState->filaPrioridade3, pcbTable);
            runningState->iPcbTable = -1;
            return 0;
        }
    } else {
        do {
            ticket = PegaTicket(ticketsSorteados);
            if (ticket == -1) {
                printf("Erro posicao nao existe ou acabaram os tickets!\n");
                return 0;
            }

            if (!EhVazia(&readyState->filaPrioridade0)) {
                for (int i = readyState->filaPrioridade0.Frente; i <= readyState->filaPrioridade0.Tras - 1; i++) {
                    if (TicketExiste(&pcbTable->vetor[readyState->filaPrioridade0.vetor[i]].tickets, ticket)) {
                        runningState->iPcbTable = readyState->filaPrioridade0.vetor[i];
                        RemoverProcessoFila(&readyState->filaPrioridade0, runningState->iPcbTable);
                        printf("Processo de PID %i escolhido!\n", pcbTable->vetor[runningState->iPcbTable].pid);
                        pegou = 1;
                    }
                }
            } else if (!EhVazia(&readyState->filaPrioridade1) && EhVazia(&readyState->filaPrioridade0)) {
                for (int i = readyState->filaPrioridade1.Frente; i <= readyState->filaPrioridade1.Tras - 1; i++) {
                    if (TicketExiste(&pcbTable->vetor[readyState->filaPrioridade1.vetor[i]].tickets, ticket)) {
                        runningState->iPcbTable = readyState->filaPrioridade1.vetor[i];
                        RemoverProcessoFila(&readyState->filaPrioridade1, runningState->iPcbTable);
                        printf("Processo de PID %i escolhido!\n", pcbTable->vetor[runningState->iPcbTable].pid);
                        pegou = 1;
                    }
                }
            } else if (!EhVazia(&readyState->filaPrioridade2) && EhVazia(&readyState->filaPrioridade1) &&
                       EhVazia(&readyState->filaPrioridade0)) {
                for (int i = readyState->filaPrioridade2.Frente; i <= readyState->filaPrioridade2.Tras - 1; i++) {
                    if (TicketExiste(&pcbTable->vetor[readyState->filaPrioridade2.vetor[i]].tickets, ticket)) {
                        runningState->iPcbTable = readyState->filaPrioridade2.vetor[i];
                        RemoverProcessoFila(&readyState->filaPrioridade2, runningState->iPcbTable);
                        printf("Processo de PID %i escolhido!\n", pcbTable->vetor[runningState->iPcbTable].pid);
                        pegou = 1;
                    }
                }
            } else if (!EhVazia(&readyState->filaPrioridade3) && EhVazia(&readyState->filaPrioridade2) &&
                       EhVazia(&readyState->filaPrioridade1) && EhVazia(&readyState->filaPrioridade0)) {
                for (int i = readyState->filaPrioridade3.Frente; i <= readyState->filaPrioridade3.Tras - 1; i++) {
                    if (TicketExiste(&pcbTable->vetor[readyState->filaPrioridade3.vetor[i]].tickets, ticket)) {
                        runningState->iPcbTable = readyState->filaPrioridade3.vetor[i];
                        RemoverProcessoFila(&readyState->filaPrioridade3, runningState->iPcbTable);
                        printf("Processo de PID %i escolhido!\n", pcbTable->vetor[runningState->iPcbTable].pid);
                        pegou = 1;
                    }
                }
            } else {
                printf("Não existe processos nas filas de prioridades!\n");
                ImprimeFila(&readyState->filaPrioridade0, pcbTable);
                ImprimeFila(&readyState->filaPrioridade1, pcbTable);
                ImprimeFila(&readyState->filaPrioridade2, pcbTable);
                ImprimeFila(&readyState->filaPrioridade3, pcbTable);
                runningState->iPcbTable = -1;
                return 0;
            }
        } while (!pegou);

    }

    processo = pcbTable->vetor[runningState->iPcbTable];

    cpu->programa.tamanho = processo.estadoProcesso.tamanho;
    cpu->programa = processo.estadoProcesso.programa;
    cpu->contadorProgramaAtual = processo.estadoProcesso.contador;
    cpu->fatiaTempo = 3;
    cpu->fatiaTempoUsada = 0;
    cpu->valorInteiro = processo.estadoProcesso.inteiro;

    return 1;

}

void ImprimirCPU(Cpu *cpu) {

    printf("\nInformacoes CPU: \n");

    printf("\nPrograma na CPU: \n");
    for (int i = 0; i < cpu->programa.tamanho; i++)
        printf("%s", cpu->programa.instrucoes[i].instrucao);
    printf("\n");
    printf("Contador de Programa Atual: %d\n", cpu->contadorProgramaAtual);
    printf("Valor Inteiro: %d\n", cpu->valorInteiro);
    printf("Fatia de Tempo Disponivel: %d\n", cpu->fatiaTempo);
    printf("Fatia de Tempo Usada: %d\n", cpu->fatiaTempoUsada);
    printf("\n\n");
}

int runCPU(Cpu *cpu, Time *time, PcbTable *pcbTable, RunningState *runningState, BlockedState *blockedState,
           ReadyState *readyState, Tickets *ticketsSorteados) {

    int flag;
    Processo processo;

    if (runningState->iPcbTable != -1) {
        if (!FEhVaziaPcbTable(pcbTable)) {
            processo = pcbTable->vetor[runningState->iPcbTable];
        } else {
            printf("Não existem processos na tabela!\n");
            return 0;
        }
    } else { /* Segunda checagem para comprovar que ainda não existe processo disponível depois do comando U. */
        if (escalonador(cpu, readyState, runningState, pcbTable, ticketsSorteados) == 0 &&
            runningState->iPcbTable == -1)
            return 0;
        else
            processo = pcbTable->vetor[runningState->iPcbTable];
    }

    strcpy(processo.estado, "EM EXECUCAO");

    pcbTable->vetor[runningState->iPcbTable] = processo;

    printf("Executando processo de PID %i.\n", processo.pid);

    flag = executarInstrucao(cpu, time, runningState, pcbTable, blockedState, readyState, &processo, ticketsSorteados);

    if (flag == 0) {
        if (!escalonador(cpu, readyState, runningState, pcbTable, ticketsSorteados)) {
            return 0;
        }
    } else if (flag == 1) {
        switch (processo.prioridade) {
            case 0:
                cpu->fatiaTempoUsada += 1;
                break;
            case 1:
                cpu->fatiaTempoUsada += 2;
                break;
            case 2:
                cpu->fatiaTempoUsada += 4;
                break;
            case 3:
                cpu->fatiaTempoUsada += 8;
                break;
            default:
                printf("Erro ao atualizar fatia de tempo CPU!\n");
        }

        /* Atualizando processo simulado */
        processo.estadoProcesso.inteiro = cpu->valorInteiro;
        processo.estadoProcesso.contador = cpu->contadorProgramaAtual;
        processo.tempoCPU = cpu->fatiaTempoUsada; // Time ou fatia de tempo?
        processo.estadoProcesso.programa = cpu->programa;
        processo.estadoProcesso.tamanho = cpu->programa.tamanho; // Necessário quando instrução R é executada.
        pcbTable->vetor[runningState->iPcbTable] = processo;

        ImprimirCPU(cpu);

        if (cpu->fatiaTempoUsada >= cpu->fatiaTempo) {
            processo.prioridade += 1;
            strcpy(processo.estado, "BLOQUEADO");
            pcbTable->vetor[runningState->iPcbTable] = processo;
            ImprimePcbTable(pcbTable);
            Enfileira(&blockedState->filaBlockedState, runningState->iPcbTable);
            ImprimeFila(&blockedState->filaBlockedState, pcbTable);
            ImprimePcbTable(pcbTable);
            escalonador(cpu, readyState, runningState, pcbTable, ticketsSorteados);
        }
    }


    return 1;

}

// Implementação Fila Arranjo

void FFVazia(Fila *fila) {
    fila->Frente = 0;
    fila->Tras = fila->Frente;
}

int EhVazia(Fila *fila) { return (fila->Frente == fila->Tras); }

void Enfileira(Fila *fila, int indiceProcesso) {
    if ((fila->Tras + 1) % MAXTAM == fila->Frente)
        printf("Erro %s esta cheia\n", fila->nome);
    else {
        fila->vetor[fila->Tras] = indiceProcesso;
        //printf("\nProcesso de PID %i adicionado a FILA PRONTO!\n", processo->pid);
        fila->Tras = (fila->Tras + 1) % MAXTAM;
    }
}

int Desenfileira(Fila *fila) {
    int indiceProcesso;
    if (EhVazia(fila)) {
        printf("\nErro %s esta vazia\n", fila->nome);
        return -1;
    } else {
        indiceProcesso = fila->vetor[fila->Frente];
        fila->Frente = (fila->Frente + 1) % MAXTAM;
        return indiceProcesso;
    }
}

void AtualizaFila(Fila *fila, int indiceProcesso) {
    for (int i = fila->Frente; i <= fila->Tras - 1; i++) {
        if (fila->vetor[i] >= (indiceProcesso + 1)) {
            fila->vetor[i] = fila->vetor[i] - 1;
        }
    }
}

void RemoverProcessoFila(Fila *fila, int indiceProccesso) {
    printf("Frente: %d\n", fila->Frente);
    printf("Tras: %d\n", fila->Tras);
    for (int i = fila->Frente; i <= fila->Tras - 1; i++) {
        if (fila->vetor[i] == indiceProccesso) {
            for (int j = i; i < fila->Tras - 1; i++)
                fila->vetor[j] = fila->vetor[i + 1];
        }
    }
    fila->Tras--;
}

void ImprimeFila(Fila *fila, PcbTable *pcbTable) {
    int Aux;
    printf("%s:\n\n", fila->nome);
    for (Aux = fila->Frente; Aux <= fila->Tras - 1; Aux++) {
        printf("PID: %i\n", pcbTable->vetor[fila->vetor[Aux]].pid);
        printf("PID Pai: %i\n", pcbTable->vetor[fila->vetor[Aux]].pid_pai);
        printf("Estado: %s\n", pcbTable->vetor[fila->vetor[Aux]].estado);
        printf("Tempo CPU: %d\n", pcbTable->vetor[fila->vetor[Aux]].tempoCPU);
        printf("Tempo Inicio: %d\n", pcbTable->vetor[fila->vetor[Aux]].timeInicio);
        printf("Prioridade: %d\n", pcbTable->vetor[fila->vetor[Aux]].prioridade);
        printf("Valor Inteiro: %d\n", pcbTable->vetor[fila->vetor[Aux]].estadoProcesso.inteiro);
        printf("Contador de Programa: %d\n", pcbTable->vetor[fila->vetor[Aux]].estadoProcesso.contador);
        printf("Programa: \n");

        for (int i = 0; i < pcbTable->vetor[fila->vetor[Aux]].estadoProcesso.tamanho; i++)
            printf("%s", pcbTable->vetor[fila->vetor[Aux]].estadoProcesso.programa.instrucoes[i].instrucao);

        printf("\n\n");
    }
}

// Implementação Lista Arranjo

void FLVaziaPcbTable(PcbTable *pcbTable) {
    pcbTable->Primeiro = 0;
    pcbTable->Ultimo = pcbTable->Primeiro;
}

void FLVaziaTickets(Tickets *tickets) {
    tickets->Primeiro = 0;
    tickets->Ultimo = tickets->Primeiro;
}

int FEhVaziaPcbTable(PcbTable *pcbTable) {
    return (pcbTable->Primeiro == pcbTable->Ultimo);
}

int FEhVaziaTickets(Tickets *tickets) {
    return (tickets->Primeiro == tickets->Ultimo);
}

int InserePcbTable(PcbTable *pcbTable, Processo processo) {
    if (pcbTable->Ultimo > MAXTAM) {
        printf("Lista esta cheia\n");
        return -1;
    } else {
        pcbTable->vetor[pcbTable->Ultimo] = processo;
        printf("\nProcesso de PID %i adicionado a Tabela de Processos (PcbTable) na posicao %d!\n", processo.pid,
               pcbTable->Ultimo);
        pcbTable->Ultimo++;
        return pcbTable->Ultimo - 1;
    }
}

int TicketExiste(Tickets *tickets, int ticket) {
    for (int i = tickets->Primeiro; i <= (tickets->Ultimo - 1); i++) {
        if (tickets->vetor[i] == ticket)
            return 1;
    }
    return 0;
}

int InsereTickets(Tickets *tickets, int ticket) {
    if (tickets->Ultimo > MAXTAM) {
        printf("Lista esta cheia\n");
        return 0;
    } else {
        tickets->vetor[tickets->Ultimo] = ticket;
        tickets->Ultimo++;
        return 1;
    }
}

int PegaTicket(Tickets *tickets) {
    int indice = (rand() % tickets->Ultimo - 1);
    if (FEhVaziaTickets(tickets) || indice >= tickets->Ultimo) {
        printf("Erro posicao nao existe ou acabaram os tickets!\n");
        return -1;
    }
    return tickets->vetor[indice];
}

void ImprimeTickets(Tickets *tickets) {
    int Aux;

    printf("\nLista de Tickets:\n\n");
    for (Aux = tickets->Primeiro; Aux <= (tickets->Ultimo - 1); Aux++) {
        printf("%d ", tickets->vetor[Aux]);
    }

    printf("\n\n");

}

int RetiraPcbTable(PcbTable *pcbTable, int indice, Processo *processo) {
    int Aux;

    if (FEhVaziaPcbTable(pcbTable) || indice >= pcbTable->Ultimo) {
        printf("Erro posicao nao existe PcbTable\n");
        return 0;
    }
    *processo = pcbTable->vetor[indice];
    pcbTable->Ultimo--;
    for (Aux = indice; Aux < pcbTable->Ultimo; Aux++)
        pcbTable->vetor[Aux] = pcbTable->vetor[Aux + 1];
    return 1;
}

void ImprimePcbTable(PcbTable *pcbTable) {
    int Aux;

    printf("\nLista de Processos na Tabela:\n\n");
    for (Aux = pcbTable->Primeiro; Aux <= (pcbTable->Ultimo - 1); Aux++) {
        printf("PID: %i\n", pcbTable->vetor[Aux].pid);
        printf("PID Pai: %i\n", pcbTable->vetor[Aux].pid_pai);
        printf("Estado: %s\n", pcbTable->vetor[Aux].estado);
        printf("Tempo CPU: %d\n", pcbTable->vetor[Aux].tempoCPU);
        printf("Tempo Inicio: %d\n", pcbTable->vetor[Aux].timeInicio);
        printf("Prioridade: %d\n", pcbTable->vetor[Aux].prioridade);
        printf("Valor Inteiro: %d\n", pcbTable->vetor[Aux].estadoProcesso.inteiro);
        printf("Contador de Programa: %d\n", pcbTable->vetor[Aux].estadoProcesso.contador);
        printf("Programa: \n");

        for (int i = 0; i < pcbTable->vetor[Aux].estadoProcesso.tamanho; i++)
            printf("%s", pcbTable->vetor[Aux].estadoProcesso.programa.instrucoes[i].instrucao);

        printf("\n\n");
    }

}