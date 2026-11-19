#include <stdio.h>
#include <stdlib.h>
#include "gerador.h"

// pasta onde os mapas sao salvos
#define PASTA "mapa"

// valor maximo de dano e de cura
#define MAX_MONSTRO 20
#define MAX_POCAO 20

// chance de cada celula (o resto eh pocao, 25%)
#define CHANCE_VAZIA 50
#define CHANCE_MONSTRO 25

int gerarMapa(int linhas, int colunas, int hpInicial, int percParedes, char *nomeArquivo, unsigned int semente){
    char caminho[200];
    int inicioL, inicioC, fimL, fimC;
    int i, j, r, tentativas;
    FILE *f;

    // valida os parametros
    // precisa de pelo menos 2 celulas pra caber o I e o F
    if (linhas <= 0 || colunas <= 0 || (linhas == 1 && colunas == 1)){
        fprintf(stderr, "A caverna precisa ter pelo menos 2 celulas (uma para I e outra para F)\n");
        return 0;
    }
    if (hpInicial <= 0){
        fprintf(stderr, "A vida inicial precisa ser maior que 0\n");
        return 0;
    }
    if (percParedes < 0 || percParedes > 100){
        fprintf(stderr, "A porcentagem de paredes tem que estar entre 0 e 100\n");
        return 0;
    }

    // monta o caminho tipo mapa/nome.txt
    snprintf(caminho, sizeof(caminho), "%s/%s", PASTA, nomeArquivo);

    f = fopen(caminho, "w");
    if (f == NULL){
        // provavelmente a pasta nao existe
        fprintf(stderr, "Erro ao criar o arquivo %s (a pasta %s existe?)\n", caminho, PASTA);
        return 0;
    }

    // a semente deixa gerar o mesmo mapa de novo
    srand(semente);

    // I fica na borda de baixo ou da direita
    if (rand() % 2 == 0){
        inicioL = linhas - 1;
        inicioC = rand() % colunas;
    }else{
        inicioL = rand() % linhas;
        inicioC = colunas - 1;
    }

    // F fica na borda de cima ou da esquerda, e precisa estar acima/esquerda do I
    // (o estudante so anda pra cima e pra esquerda)
    // sorteia ate 100 vezes ate achar uma posicao boa
    fimL = 0;
    fimC = 0;
    for (tentativas = 0; tentativas < 100; tentativas++){
        if (rand() % 2 == 0){
            // borda de cima
            fimL = 0;
            fimC = rand() % colunas;
        }else{
            // borda da esquerda
            fimL = rand() % linhas;
            fimC = 0;
        }
        if (fimL <= inicioL && fimC <= inicioC && !(fimL == inicioL && fimC == inicioC)){
            break;
        }
    }

    // se nao achou posicao boa (caverna pequena), coloca o F no 0 0 e o I no canto oposto
    if (fimL > inicioL || fimC > inicioC || (fimL == inicioL && fimC == inicioC)){
        fimL = 0;
        fimC = 0;
        inicioL = linhas - 1;
        inicioC = colunas - 1;
    }

    // primeira linha: tamanho da matriz e vida inicial
    fprintf(f, "%d %d %d\n", linhas, colunas, hpInicial);

    // escreve a matriz celula por celula
    for (i = 0; i < linhas; i++){
        for (j = 0; j < colunas; j++){
            if (i == inicioL && j == inicioC){
                fprintf(f, "I");
            }else if (i == fimL && j == fimC){
                fprintf(f, "F");
            }else if (rand() % 100 < percParedes){
                // parede
                fprintf(f, "X");
            }else{
                // sorteia o que vai ter na celula
                r = rand() % 100;
                if (r < CHANCE_VAZIA){
                    fprintf(f, "0");
                }else if (r < CHANCE_VAZIA + CHANCE_MONSTRO){
                    // monstro (negativo)
                    fprintf(f, "-%d", 1 + rand() % MAX_MONSTRO);
                }else{
                    // pocao (positivo)
                    fprintf(f, "+%d", 1 + rand() % MAX_POCAO);
                }
            }

            // espaco entre as celulas, menos na ultima da linha
            if (j < colunas - 1){
                fprintf(f, " ");
            }
        }
        fprintf(f, "\n");
    }

    fclose(f);
    printf("Caverna %dx%d gerada em %s (semente %u)\n", linhas, colunas, caminho, semente);
    return 1;
}