#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <errno.h>

void ls(const char *caminho);

int main(int argc, char *argv[]) {
    int i;

    /*
     * Percorre cada um dos argumentos passados
     * pela linha de comando. 
     */
    for (i = 1; i < argc; i++) {
        ls(argv[i]);

        // Caso seja último argumento, não é necessário quebrar a linha
        if (i != argc - 1) printf("\n");   
    }

    return 0;
}

void ls(const char *caminho) {
    struct dirent *dptr; // Ponteiro para percorrer o diretório
    DIR *diretorio = opendir(caminho); // Abre o diretório
    
    // O tamanho máximo do nome do arquivo no Linux é 256 bytes
    char nomeDoArquivo[256]; 

    // Verifica se o diretório é existente
    if (diretorio == NULL) { 
        printf("Não foi possível abrir o diretório %s\n", caminho);
        return;
    }

    /*
     * Caso o caminho exista, percorre o diretório
     * e imprime cada um dos arquivos.
     */
    printf("Arquivos do diretório %s\n", caminho);
    while ((dptr = readdir(diretorio)) != NULL) {
        strcpy(nomeDoArquivo, dptr -> d_name);

        // Ignora os arquivos '.', '..' e ocultos' 
        if (nomeDoArquivo[0] == '.') continue; 
        printf("%s ", nomeDoArquivo);
    }
    printf("\n");

    // Caso ocorra um erro durante a leitura, notifica o usuário
    if (errno != 0) {
        printf("[Erro]: Problema durante a leitura do diretorio %s\n", caminho);
        return;
    }

    // Fecha o diretório
    closedir(diretorio);
}