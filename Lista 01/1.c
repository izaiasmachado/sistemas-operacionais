#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
    pid_t pid = fork();

    int i;
    int half = (argc) / 2; // Pega a quantidade de argumentos e divide por 2

    if (pid != 0) { // Processo Pai
        printf("Argumentos do processo Pai: \n");

        for (i = 0; i < half; i++) { // Argumentos do começo até a metade
            printf("- %s\n", argv[i]);
        }

        wait(0); // Aguarda até o fim do processo Filho
    } else { // Processo Filho
        printf("Argumentos do processo Filho: \n");
        
        for (i = half; i < argc; i++) { // Argumentos da metade até o final
            printf("- %s\n", argv[i]);
        }
    }

    return 0;
}