#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>

void printTerminal(char* text);

int main(int argc, char *argv[]){
    pid_t pid = fork();

    if (pid != 0) {
        printTerminal("Pai "); 
        wait(0); // Processo pai aguarda os filhos.

        /*
            Após finalizado todos os processos filhos, 
            é feita uma quebra de linha.
        */
        printf("\n");
    } else {
        pid_t pid2 = fork();            
        
        if (pid2 != 0) {
            printTerminal("-> Filho ");
            wait(0);
        } else {
            printTerminal("-> Filho1");
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