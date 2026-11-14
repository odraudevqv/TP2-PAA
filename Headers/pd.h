#ifndef PD_H
#define PD_H

#include "caverna.h"

typedef struct pd{
    int linhas;
    int colunas;
    int **tabela;
}PD;

int inicializarPD(PD *pd, int linhas, int colunas);

int calcularMelhorCaminho(Caverna *c, PD *pd);

void liberarPD(PD *pd);

#endif