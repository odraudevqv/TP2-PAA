// gerador.c
// Funcao que gera um mapa (caverna) aleatorio e salva na pasta "mapa".
//
// O arquivo gerado segue o mesmo formato do trabalho:
//   - primeira linha: linhas colunas hpInicial
//   - depois a matriz: numero = monstro (negativo) ou pocao (positivo), I = inicio, F = fim
//   - extra: X = parede

#include <stdio.h>
#include <stdlib.h>
#include "gerador.h"

// Nome da pasta onde os mapas sao salvos (ela precisa ja existir)
#define PASTA "mapa"

// Valores maximos de dano e de cura de uma celula (da pra mudar aqui se quiser)
#define MAX_MONSTRO 20
#define MAX_POCAO 20

// Chance (em %) de cada tipo de celula quando ela nao e parede.
// A soma tem que dar 100: 35% vazia + 40% monstro + 25% pocao
#define CHANCE_VAZIA 35
#define CHANCE_MONSTRO 40

int gerarMapa(int linhas, int colunas, int hpInicial, int percParedes, char *nomeArquivo, unsigned int semente){
    char caminho[200];
    int inicioL, inicioC, fimL, fimC;
    int i, j, r, tentativas;
    FILE *f;

    // Confere se os parametros fazem sentido
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

    // Monta o caminho do arquivo: mapa/nomeArquivo
    snprintf(caminho, sizeof(caminho), "%s/%s", PASTA, nomeArquivo);

    f = fopen(caminho, "w");
    if (f == NULL){
        fprintf(stderr, "Erro ao criar o arquivo %s (a pasta %s existe?)\n", caminho, PASTA);
        return 0;
    }

    srand(semente);

    // Escolhe a posicao do I em alguma borda de baixo ou da direita
    if (rand() % 2 == 0){
        inicioL = linhas - 1;
        inicioC = rand() % colunas;
    }else{
        inicioL = rand() % linhas;
        inicioC = colunas - 1;
    }

    // Escolhe a posicao do F em alguma borda de cima ou da esquerda.
    // O F precisa estar acima/esquerda do I, porque o estudante so anda pra cima e pra esquerda.
    // Tenta sortear varias vezes ate achar uma posicao boa.
    fimL = 0;
    fimC = 0;
    for (tentativas = 0; tentativas < 100; tentativas++){
        if (rand() % 2 == 0){
            fimL = 0;
            fimC = rand() % colunas;
        }else{
            fimL = rand() % linhas;
            fimC = 0;
        }
        if (fimL <= inicioL && fimC <= inicioC && !(fimL == inicioL && fimC == inicioC)){
            break;
        }
    }

    // Se mesmo assim o F ficou no mesmo lugar do I (pode acontecer em caverna pequena),
    // joga o I para o canto de baixo a direita, que nunca coincide com o 0 0.
    if (fimL > inicioL || fimC > inicioC || (fimL == inicioL && fimC == inicioC)){
        fimL = 0;
        fimC = 0;
        inicioL = linhas - 1;
        inicioC = colunas - 1;
    }

    // Primeira linha do arquivo
    fprintf(f, "%d %d %d\n", linhas, colunas, hpInicial);

    // Escreve a matriz celula por celula
    for (i = 0; i < linhas; i++){
        for (j = 0; j < colunas; j++){
            if (i == inicioL && j == inicioC){
                fprintf(f, "I");
            }else if (i == fimL && j == fimC){
                fprintf(f, "F");
            }else if (rand() % 100 < percParedes){
                fprintf(f, "X");                       // parede
            }else{
                r = rand() % 100;
                if (r < CHANCE_VAZIA){
                    fprintf(f, "0");                   // celula vazia
                }else if (r < CHANCE_VAZIA + CHANCE_MONSTRO){
                    fprintf(f, "-%d", 1 + rand() % MAX_MONSTRO);   // monstro (tira vida)
                }else{
                    fprintf(f, "+%d", 1 + rand() % MAX_POCAO);     // pocao (da vida)
                }
            }

            // Espaco entre as celulas, mas nao depois da ultima da linha
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
