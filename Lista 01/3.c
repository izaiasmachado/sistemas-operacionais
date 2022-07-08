#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
    pid_t pid = fork();

    if (pid != 0) {
        wait(0); // Fica aqui até o fim do processo filho
        printf("Processo filho finalizado!\n");
    } else {
        char* path =  "/snap/bin/spotify";
        /*
            A função execl substitui o processo inicializado
            por outro processo, ou seja, é sobrescrito.
        */
        execl(path, path, NULL, NULL, NULL);
    }
    
    return 0;
}