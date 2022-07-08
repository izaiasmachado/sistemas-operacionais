#include <stdio.h>
#include <stdlib.h>
#include "processos.h"

void imprimeTemposDeEspera(Processo processos[], int quantidade);
void imprimeDadosDaExecucao(Processo processos[], int quantidade);

Processo *lerFilaDeExecucaoDeProcessos(int *quantidade);
int escolheAlgoritmoDeEscalonamento(Processo fila[], int quantidade);

int main() {
    int quantidade;
    Processo *fila, *snapshot;
    char end = 's', usarMesmaFila = 'n';

    while (end == 'S' || end == 's') {

        /*
         * Ao fim do programa o usuário é perguntado se ele quer receber uma
         * nova fila de processos.
        */
        if (usarMesmaFila == 'N' || usarMesmaFila == 'n') {
            fila = lerFilaDeExecucaoDeProcessos(&quantidade);
        }

        /**
         * Como após a snapshot é necessário que a lista de processos seja alterada,
         * é feita uma snapshot para que o estado inicial dela seja mantido.
         *
         * Com isso, é possível reutilizar uma fila de processos digitada
         * para mais de um algoritmo de escalonamento.
         */
        snapshot = tiraSnapshot(fila, quantidade);


        /*
         * Se um algoritmo for escolhido, imprime os tempos de espera de cada algoritmo
         * e outros dados sobre a execução.
        */
        int algoritmoEscolhido = escolheAlgoritmoDeEscalonamento(snapshot, quantidade);
        if (algoritmoEscolhido) {
            imprimeTemposDeEspera(snapshot, quantidade);
            imprimeDadosDaExecucao(snapshot, quantidade);
        }

        printf("Deseja continuar? [S/N] ");
        scanf(" %c", &end);

        if (end == 'S' || end == 's') {
            printf("Deseja usar a mesma lista de processos? [S/N] ");
            scanf(" %c", &usarMesmaFila);
        }

    }

    printf("Programa finalizado\n");
    
    free(fila);
    free(snapshot);

    return 0;
}

void imprimeTemposDeEspera(Processo processos[], int quantidade) {
    int i;
    
    printf("\n===== Tempo de espera =====\n");
    for (i = 0; i < quantidade; i++) {
        printf("Tempo total de espera do processo %d: %ds\n", processos[i].id, processos[i].espera);
    }   
}

void imprimeDadosDaExecucao(Processo processos[], int quantidade) {
    int i, tempoTotalDeExecucao = 0;
    double esperaMedia;
    int total, execucao = 0, espera = 0;

    for (i = 0; i < quantidade; i++) {
        espera += processos[i].espera;
        execucao += processos[i].execucao;
    }

    esperaMedia = espera / (double) quantidade;

    /*
     * O tempo total consumido pelo algoritmo é o tempo total de execução
     * somado ao tempo total de espera.     
     */
    total = execucao + espera;
    
    printf("\n");
    printf("Tempo total gasto: %ds\n", total);
    printf("Tempo de espera total: %ds\n", espera);
    printf("Tempo de espera médio: %.2lf (s/processo)\n", esperaMedia);
}

Processo *lerFilaDeExecucaoDeProcessos(int *quantidade) {
    Processo processoRecebido, *fila;
    int i, tempoTotalDeExecucao;

    printf("Digite a quantidade de processos que estão na fila de execução: ");
    scanf("%d", quantidade);

    // Aloca uma nova fila
    fila = (Processo *) malloc((*quantidade) * sizeof(Processo));
    
    // Pede que o usuário digite os tempos de execução
    for (i = 0; i < *quantidade; i++) {
        printf("Digite o tempo de execução do processo %d: ", i + 1);
        scanf("%d", &tempoTotalDeExecucao);

        processoRecebido.id = i + 1;
        processoRecebido.finalizado = 0;
        processoRecebido.executado = 0;
        processoRecebido.execucao = tempoTotalDeExecucao;
        fila[i] = processoRecebido;
    }

    return fila;
}

// Pede que o usuário digite o algoritmo de escalonamento e executa o escolhido
int escolheAlgoritmoDeEscalonamento(Processo *fila, int quantidade) {
    enum ALGORITMO { FIFO, SJF, RR } algoritmoEscolhido;
    int quantum, opcaoEscolhida;

    printf("[0] - First In First Out\n[1] - Shortest Job First\n[2] - Round Robin\n");
    printf("Digite o algoritmo de escalonamento: ");
    scanf("%d", &algoritmoEscolhido);

    switch(algoritmoEscolhido) {
        case FIFO:
            firstInFirstOut(fila, quantidade);
            break;
        case SJF:
            shortestJobFirst(fila, quantidade);
            break;
        case RR:
            printf("Digite o tempo de quantum: ");
            scanf("%d", &quantum);
            roundRobin(fila, quantidade, quantum);
            break;
        default:
            printf("Operação inválida!\n");
            return 0;
            break;
    }

    return 1;
}