#include "processos.h"

/* 
 * Tira uma cópia e retorna ponteiro apontando 
 * para a snapshot da fila de procesos original
 */ 
Processo *tiraSnapshot(Processo original[], int quantidade) {
    Processo *snapshot = (Processo*) malloc(quantidade * sizeof(Processo));
    
    for (int i = 0; i < quantidade; i++) {
        snapshot[i] = original[i];
    }

    return snapshot;
}

// Troca valores de memória de ponteiros de processos
void swap(Processo *processo1, Processo *processo2) {
    Processo aux = *processo1;
    *processo1 = *processo2;
    *processo2 = aux;
}

int quantidadeProcessosFinalizados(Processo processos[], int quantidade) {
    int finalizados = 0;
    for (int i = 0; i < quantidade; i++) {
        if (processos[i].finalizado) finalizados++;
    }
    return finalizados;
}

int todosProcessosFinalizados(Processo processos[], int quantidade) {
    int i;
    Processo processo;
    
    for (i = 0; i < quantidade; i++) {
        processo = processos[i];
        if (!processo.finalizado) return 0;
    }

    return 1;
}

int ehUltimaExecucao(Processo processos[], int quantidade) {
    int finalizados = quantidadeProcessosFinalizados(processos, quantidade);
    return finalizados == quantidade - 1;
}

void firstInFirstOut(Processo processos[], int quantidade) {
    /*
     * O FIFO é um caso especial do roundRobin em que o quantum 
     * tem um tempo infinito. 
     *
     * O o algoritmo foi implementado de modo que quando o 
     * quantum for igual a -1, seu valor é considerado como infinito.
     */

    roundRobin(processos, quantidade, -1);
}

void shortestJobFirst(Processo processos[], int quantidade) {
    int i, j;
    Processo *processoA, *processoB;
 
    /* 
     * Ordena os processos por tempo de execução, de modo que o 
     * processo com menor tempo de execução fique no início da fila.
     */

    for (i = 0; i < quantidade - 1; i++) {
        processoA = processos + i;

        for (j = i; j < quantidade; j++) {            
            processoB = processos + j;

            if (processoA -> execucao > processoB -> execucao) {
                swap(processoA, processoB);
            }
        }
    }

    // Realiza o mesmo procedimento do FIFO, porém com os tempos ordenados
    firstInFirstOut(processos, quantidade);
}

void roundRobin(Processo *processos, int quantidade, int quantum) {
    int i, indiceProximo = 0;
    int tempo = 0, tempoParaExecucao, tempoRestante;
    Processo *processo;

    /*
     * Realiza o algoritmo de escalonamento até
     * que todos os processos sejam finalizados.    
     */
    while (!todosProcessosFinalizados(processos, quantidade)) {
        /*
         * Caso o valor do próximo processo em execução seja 
         * maior que o indice do último processo,
         * seta o próximo processo como sendo zero. 
         */
        if (indiceProximo >= quantidade) indiceProximo = 0;
        processo = processos + indiceProximo;
        indiceProximo++;

        // Se o processo já foi finalizado, pula para o próximo processo
        if (processo -> finalizado) continue; 

        tempoRestante = processo -> execucao - processo -> executado;
        tempoParaExecucao = quantum;

        if (tempoRestante < quantum || quantum == -1 || ehUltimaExecucao(processos, quantidade)) {
            tempoParaExecucao = tempoRestante;
        }

        for (i = 0; i < tempoParaExecucao; i++) {
            printf("T: %ds - Processo %d - Tempo Executado: %ds - Tempo Total Restante: %ds\n",
                tempo,
                processo -> id,
                processo -> executado,
                processo -> execucao - processo -> executado
            );
            
            sleep(1);

            tempo += 1;
            processo -> executado += 1;
        }
        
        /*
         * Se ainda houver tempo restante para execução desse processo, 
         * pula para o próximo processo, porque o processo atual já executou 
         * todo o tempo do quantum.
         */
        if (processo -> executado < processo -> execucao) continue;

        /*
         * Se não houver mais tempo para execução, seta o processo como 
         * finalizado, define o tempo de retorno e também o tempo de espera.
         */
        processo -> finalizado = 1;
        processo -> retorno = tempo;
        processo -> espera = processo -> retorno - processo -> execucao;
        printf("T: %ds - Processo %d - Finalizou totalmente sua execução!\n", 
            tempo, 
            processo -> id
        );
    }
}