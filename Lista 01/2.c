#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>

void printTerminal(char* text);

int main(int argc, char* argv[]) {
    int i;
    pid_t pid = fork();

    if (pid != 0) { // Código do Processo Pai
        printTerminal("Comandos a serem executados: \n");
        
        for (i = 1; i < argc; i++) {
            char message[255] = "";
            strcat(message, " - ");
            strcat(message, argv[i]);
            strcat(message, "\n");
            printTerminal(message);
        }

        printf("\n");
        wait(0);
        
        printf("Execução dos comandos finalizada!\n");
    } else { // Código para quando não é o processo Pai em execução
        for (i = 1; i < argc; i++) { // Percorre os argumentos
            pid_t pid2 = fork();

            if (pid2 != 0) { // Código do processo Filho 0
                printf("Executando %s\n", argv[i]);
                wait(0); // Aguarda processo Filho 1
        	    printf("Fim do comando %s\n\n", argv[i]);
            } else { // Código do processo Filho 1
                char path[] = "";
                strcat(path, "/bin/");
                strcat(path, argv[i]);
                execl(path, path, NULL, NULL, NULL);
            }
        }
    }


    return 0;
}

/*
    Função para escrever no terminal mesmo
    quando fora do processo pai
*/
void printTerminal(char* text) {
    write(1, text, strlen(text));
}