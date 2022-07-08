#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#define NUMBER_OF_THREADS 2

struct dados_thread {
    int thread_id;
    int n;
    int *resultado;
};

typedef struct dados_thread DadosThread;

int ehPrimo(int numero) {
    int i;
    if (numero == 0 || numero == 1) return 0;
    
    for (i = 2; i < numero; i++) {
        if (numero % i == 0) return 0;
    }

    return 1;
}

void *calculaNumerosPrimos(void* x) {
	DadosThread* xptr = (DadosThread*) x;
	int i;
    int n = xptr -> n;
    int thread_id = xptr -> thread_id;

    // Aloca-se o ponteiro que deve armazenar o resultado
    xptr -> resultado = (int*) malloc(sizeof(int));

    // O ponteiro quantidade aponta para o ponteiro resultado da thread
	int* quantidade = xptr -> resultado; 
    *quantidade = 0;

    int batchSize = (n / NUMBER_OF_THREADS);
    int start = thread_id * batchSize;
    int end = (thread_id + 1) * batchSize;

    // De 0 a 6
    // batch size = 6 / 2 = 3
    // batch thread 0 => (i = 0 * 3; i < 3; i++) // 0 1 2
    // batch thread 1 => (i = 1 * 3; i < 6; i++) // 3 4 5
    for (i = start; i < end; i++) {
        if (ehPrimo(i)) *quantidade += 1;
    }

    /*
     * Caso não seja possível dividir igualmente, o último thread
     * deve calcular o restante.   
     */
    if (thread_id != NUMBER_OF_THREADS - 1) return NULL;

    for (i = end; i <= n; i++) {
        if (ehPrimo(i)) *quantidade += 1;
    }

	return NULL;
}

int main() {
	pthread_t threads[NUMBER_OF_THREADS];
    DadosThread dados[NUMBER_OF_THREADS];
    int n, i, quantidade = 0;

    printf("Digite o número maior que 0: ");
    scanf("%d", &n);

    if (n <= 0) {
        printf("Número inválido\n");
        return 0;
    }

    for (i = 0; i < NUMBER_OF_THREADS; i++) {
        dados[i].thread_id = i;
        dados[i].n = n;
        pthread_create(&threads[i], NULL, calculaNumerosPrimos, (void *) &dados[i]);
    	pthread_join(threads[i], NULL);
    }

    for (i = 0; i < NUMBER_OF_THREADS; i++) {
        printf("Resultado da Thread %d: %d\n", 
            dados[i].thread_id + 1, 
            *(dados[i].resultado)
        );
        quantidade += *(dados[i].resultado);
    }

    printf("\nEntre 0 e %d há %d números primos.\n", n, quantidade);
    return 0;
}