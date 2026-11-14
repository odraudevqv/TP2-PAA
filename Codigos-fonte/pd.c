#include <stdio.h>
#include <stdlib.h>
#include "pd.h"

int inicializarPD(PD *pd, int linhas, int colunas){
    int i, j;

    pd -> linhas = linhas;
    pd -> colunas = colunas;

    pd -> tabela = (int**)malloc(linhas * sizeof(int*));
    if (pd -> tabela == NULL){
        fprintf(stderr, "Erro ao alocar memoria para a tabela de PD\n");
        return 0;
    }

    for (i = 0; i < linhas; i++){
        pd -> tabela[i] = (int*)malloc(colunas * sizeof(int));
        if (pd -> tabela[i] == NULL){
            for (j = 0; j < i; j++){
                free(pd -> tabela[j]);
            }
            free(pd -> tabela);
            return 0;
        }
    }

    // Inicializa todas as posicoes com -1 (indicando celula inalcancavel ou morte)
    for (i = 0; i < linhas; i++){
        for (j = 0; j < colunas; j++){
            pd -> tabela[i][j] = -1;
        }
    }

    return 1;
}

int calcularMelhorCaminho(Caverna *c, PD *pd){
    int i, j;
    int hpBaixo, hpDireita, maiorHpAnterior, hpAtual;

    // Verifica se os pontos de vida iniciais e se F esta a cima/esquerda de I
    if (c -> hpInicial <= 0 || c -> inicioL < c -> fimL || c -> inicioC < c -> fimC){
        return 0;
    }

    // Posicao inicial do estudante recebe o HP inicial
    pd -> tabela[c -> inicioL][c -> inicioC] = c -> hpInicial;

    // De I (inicioL, inicioC) ate F (fimL, fimC)
    for (i = c -> inicioL; i >= c -> fimL; i--){
        for (j = c -> inicioC; j >= c -> fimC; j--){
            
            // Pula a celula inicial
            if (i == c -> inicioL && j == c -> inicioC){
                continue;
            }

            hpBaixo = -1;
            hpDireita = -1;

            // Se veio de baixo movimenta para cima (de i + 1 para i)
            if (i + 1 <= c -> inicioL){
                hpBaixo = pd -> tabela[i + 1][j];
            }

            // Se veio da direita movimenta para a esquerda (de j + 1 para j)
            if (j + 1 <= c -> inicioC){
                hpDireita = pd -> tabela[i][j + 1];
            }

            // Escolhe o maior HP entre os dois caminhos anteriores
            if (hpBaixo > hpDireita){
                maiorHpAnterior = hpBaixo;
            }else{
                maiorHpAnterior = hpDireita;
            }

            // So avanca se estava vivo na sala anterior
            if (maiorHpAnterior > 0){
                hpAtual = maiorHpAnterior + c -> matriz[i][j];

                // Se HP maior que zero, sobreviveu
                if (hpAtual > 0){
                    pd -> tabela[i][j] = hpAtual;
                }else{
                    pd -> tabela[i][j] = -1; // Estudante morreu
                }
            }
        }
    }

    // Retorna 1 se chegou ao fim (F) com vida (> 0), ou 0 caso seja impossivel
    if (pd -> tabela[c -> fimL][c -> fimC] > 0){
        return 1;
    }

    return 0;
}

void liberarPD(PD *pd){
    int i;
    for (i = 0; i < pd -> linhas; i++){
        free(pd -> tabela[i]);
    }
    free(pd -> tabela);
    pd -> tabela = NULL;
}