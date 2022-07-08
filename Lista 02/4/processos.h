#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

struct processo {
    int id;
    int executado;
    int execucao;
    int espera;
    int retorno;
    int finalizado;
};

typedef struct processo Processo;
Processo *tiraSnapshot(Processo processos[], int quantidade);
void roundRobin(Processo *processos, int quantidade, int quantum);
void firstInFirstOut(Processo processos[], int quantidade);
void shortestJobFirst(Processo processos[], int quantidade);