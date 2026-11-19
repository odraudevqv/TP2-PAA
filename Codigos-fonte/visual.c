#include <stdio.h>
#include <stdlib.h>
#include "visual.h"

#ifdef _WIN32
#include <windows.h>
#endif

void imprimirCaminhoCriativo(Caverna *c, PD *pd, int encontrou){
    int i, j, passos;
    int hpBaixo, hpDireita;
    char *caminho;   // 1 = sala que faz parte do caminho (matriz linearizada: i * colunas + j)
#ifdef _WIN32
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD modo = 0;
    UINT codigoAnterior = GetConsoleOutputCP();
#endif

    if (!encontrou){
        printf("\nImpossivel escapar.\n");
        return;
    }

    if (c -> colunas > 60){
        printf("\nCaverna larga demais para o print.\n");
        return;
    }

    caminho = (char*)calloc(c -> linhas * c -> colunas, sizeof(char));
    if (caminho == NULL){
        fprintf(stderr, "Erro ao alocar memoria para o print\n");
        return;
    }

    // Marca o caminho de F ate I, com o mesmo desempate do escreverCaminho
    i = c -> fimL;
    j = c -> fimC;
    caminho[i * c -> colunas + j] = 1;
    passos = 1;
    while (i != c -> inicioL || j != c -> inicioC){
        hpBaixo = -1;
        hpDireita = -1;
        if (i < c -> inicioL){
            hpBaixo = pd -> tabela[i + 1][j];
        }
        if (j < c -> inicioC){
            hpDireita = pd -> tabela[i][j + 1];
        }

        if (hpBaixo > hpDireita){
            i++;
        }else{
            j++;
        }
        caminho[i * c -> colunas + j] = 1;
        passos++;
    }

#ifdef _WIN32
    SetConsoleOutputCP(65001);                     // UTF-8, para os simbolos de parede e caminho aparecerem certo
    if (GetConsoleMode(console, &modo)){
        SetConsoleMode(console, modo | 0x0004);    // liga as cores ANSI 
    }
#endif

    printf("\nCaminho encontrado (%d salas) | HP final: %d\n", passos, pd -> tabela[c -> fimL][c -> fimC]);

    for (i = 0; i < c -> linhas; i++){
        for (j = 0; j < c -> colunas; j++){
            if (i == c -> inicioL && j == c -> inicioC){
                printf("\033[1;36mI \033[0m");
            }else if (i == c -> fimL && j == c -> fimC){
                printf("\033[1;31mF \033[0m");
            }else if (c -> matriz[i][j] == PAREDE){
                printf("\033[90m██\033[0m");
            }else if (caminho[i * c -> colunas + j]){
                printf("\033[32m● \033[0m");
            }else{
                printf("  ");
            }
        }
        printf("\n");
    }
    printf("\n");

#ifdef _WIN32
    SetConsoleOutputCP(codigoAnterior);            // devolve a codificacao que o terminal usava
#endif

    free(caminho);
}