#ifndef CAVERNA_H
#define CAVERNA_H

#include <limits.h>

// Valor especial que guarda na matriz as celulas que sao PAREDE (token "X" no arquivo).
// Usei INT_MIN porque nenhuma celula normal da caverna vai ter esse valor.
#define PAREDE INT_MIN

typedef struct caverna{
    int linhas;
    int colunas;
    int hpInicial;
    int **matriz;
    int inicioL, inicioC;
    int fimL, fimC;
}Caverna;

int lerCaverna(Caverna *c, char *nomeArquivo);

void liberarCaverna(Caverna *c);


#endif