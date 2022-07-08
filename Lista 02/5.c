#define _XOPEN_SOURCE 500
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <math.h>
#include <unistd.h>
#include <ftw.h>
#include <time.h>

#define MAX_TAM_CAMINHO 1024
#define MAX_TAM_NOME 256
#define MAX_FILES 10e6

struct arquivo {
    char caminho[MAX_TAM_NOME];
    double bytes;
};

typedef struct arquivo Arquivo;
Arquivo *arquivos, arquivo;
int quantidadeDeArquivos;

double maiorTamanhoArquivo, menorTamanhoArquivo;

void percorreArvoreDiretorio(const char *caminho);
int percorreCaminho(const char *caminho, const struct stat *sb, int flag, struct FTW *ftwbuf);
void imprimeHistograma(const double larguraDaCelula);

int main(int argc, char *argv[]) {
    int arg, i;
    double larguraDaCelula;
    char path[MAX_TAM_CAMINHO] = ".";

    // Caso o usuário não tenha fornecido um caminho, o programa não executa.
    if (argc < 2) {
        printf("Caminho não fornecido.\n");
        return 0;
    }
    
    // Percorre cada um dos caminhos
    for (i = 1; i < argc; i++) {
        strcpy(path, argv[i]);
        printf("Gráfico do caminho: %s\n", path);
 
        // Fica preso aqui até que o usuário digite uma largura de célula válida
        do {
            printf("Digite a largura da célula do histograma (maior que zero): ");
            scanf("%lf", &larguraDaCelula);

            // Caso a largura de célula seja válida, sai do while.
            if (larguraDaCelula > 0) break;

            // Se não, imprime uma mensagem informando que o valor é inválido
            printf("Largura de célula inválida!!\n");
        } while (larguraDaCelula <= 0);

        // Percorre a árvore de diretórios do caminho fornecido
        percorreArvoreDiretorio(path);
        
        // Imprime o histograma
        imprimeHistograma(larguraDaCelula);
    }

    return 0;
}

void percorreArvoreDiretorio(const char *caminho) {
    int result;

    // Verifica se o caminho existe
    if (caminho == NULL) {
        printf("Não foi possível abrir o diretório %s\n", caminho);
        return;
    }

    // Esvazia a lista de arquivos
    quantidadeDeArquivos = 0;
    arquivos = (Arquivo *) malloc(sizeof(Arquivo) * MAX_FILES);

    /* 
     * Percorre a árvore de diretórios do caminho fornecido
     * e executa a função percorreCaminho para cada arquivo
     * ou diretório encontrado.
     */
    result = nftw(caminho, percorreCaminho, 20, FTW_PHYS);
    if (result >= 0) {
        errno = result;
    }

    // Caso haja um erro, notifica o usuário
    if (errno != 0) {
        printf("[Erro]: Problema durante a leitura do diretorio %s\n", caminho);
    }
}

int percorreCaminho(const char *caminho, const struct stat *sb, 
    int flag, struct FTW *ftwbuf) {
    // Caso o arquivo não seja um arquivo, não faz nada
    if (flag != FTW_F) return 0; 
    
    // Armazena o tamnho do arquivo
    arquivo.bytes = (double) sb -> st_size;

    // Aramazena o caminho do arquivo
    strcpy(arquivo.caminho, caminho);
    
    // Aramazena o menor e o maior tamanho de arquivos
    if (arquivo.bytes > maiorTamanhoArquivo || quantidadeDeArquivos == 0) {
        maiorTamanhoArquivo = arquivo.bytes;
    }
    if (arquivo.bytes < menorTamanhoArquivo || quantidadeDeArquivos == 0) {
        menorTamanhoArquivo = arquivo.bytes;
    }
    
    // Adiciona o arquivo à lista de arquivos
    arquivos[quantidadeDeArquivos++] = arquivo;    
    return 0;
}


void imprimeHistograma(const double larguraDaCelula) {
    int i, contador;
    double it, inicio, fim;
    double menorPotencia = 0;
    double menorClasse = 0;

    // Calcula a menor classe em potência da largura da célula
    if (menorTamanhoArquivo > 0) {
        menorPotencia = floor(log(menorTamanhoArquivo) / log(larguraDaCelula));
        menorClasse = pow(larguraDaCelula, menorPotencia);
    }

    /*
     * Imprime o histograma com as classes de tamanho de arquivos
     * até o arquivo de maior tamanho.
     */
    for (it = menorClasse; it <= maiorTamanhoArquivo; it += larguraDaCelula) {
        // Calcula o inicio e o fim de cada classe
        inicio = it; // inicio = 0
        fim = inicio + larguraDaCelula - 1; // fim = 1023
        contador = 0;

        // Percorre cada um dos arquivos verificando se estão dentro da classe
        for (i = 0; i < quantidadeDeArquivos; i++) {
            if (arquivos[i].bytes >= inicio && arquivos[i].bytes <= fim) contador++;
        }

        printf("[%.3lf %.3lf]: %d\n", inicio, fim, contador);
    }
}