#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "caverna.h"

int lerCaverna(Caverna *c, char *nomeArquivo){
    char token[32]; //Token para ler cada elemento da matriz. Lê tudo como string primeiro, para depois converter para inteiro
    int i, j;
    FILE *f = fopen(nomeArquivo, "r");
    if (f == NULL){
        fprintf(stderr, "Erro ao abrir o arquivo %s\n", nomeArquivo);
        return 0;
    }

    if (fscanf(f, "%d %d %d", &c -> linhas, &c -> colunas, &c -> hpInicial) != 3){
        fprintf(stderr, "Erro ao ler a primeira linha do arquivo\n");
        fclose(f);
        return 0;
    }

    c -> inicioL = -1; //Inicializa as coordenas de início e fim como -1, indicando que ainda não foram encontradas
    c -> inicioC = -1;
    c -> fimL = -1;
    c -> fimC = -1;

    c -> matriz = (int**)malloc(c -> linhas * sizeof(int*));   //Aloca a matriz
    if (c -> matriz == NULL){
        fprintf(stderr, "Erro ao alocar memoria para a matriz\n");
        fclose(f);
        return 0;
    }

    for (i = 0; i < c -> linhas; i++){
        c -> matriz[i] = (int*)malloc(c -> colunas * sizeof(int));
        if (c -> matriz[i] == NULL){
            for (j = 0; j < i; j++){
                free(c -> matriz[j]);
            }
            free (c -> matriz);
            fclose(f);
            return 0;
        }
    }

    for (i = 0; i < c -> linhas; i++){            //Lê a matriz e identifica início e saída
        for (j = 0; j < c -> colunas; j++){
            fscanf(f, "%s", token);
            if (strcmp(token, "I") == 0){
                c -> inicioL = i;
                c -> inicioC = j;
                c -> matriz[i][j] = 0;
            }else if (strcmp(token, "F") == 0){
                c -> fimL = i;
                c -> fimC = j;
                c -> matriz[i][j] = 0;
            }else if (strcmp(token, "X") == 0 || strcmp(token, "x") == 0){
                c -> matriz[i][j] = PAREDE;         //Parede: marca a celula com o valor especial PAREDE
            }else{
                c -> matriz[i][j] = atoi(token);    //Converte string para inteiro. Ex: "-20" = -20
            }
        }
    } 

    fclose(f);
    return 1;
}

void liberarCaverna(Caverna *c){
    int i; 
    for (i = 0; i < c -> linhas; i++){
        free(c -> matriz[i]);
    }
    free (c -> matriz);
    c -> matriz = NULL;
}

