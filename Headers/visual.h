#ifndef VISUAL_H
#define VISUAL_H

#include "caverna.h"
#include "pd.h"

// Desenha a caverna no terminal, com cores, mostrando o caminho que a PD encontrou.
void imprimirCaminhoCriativo(Caverna *c, PD *pd, int encontrou);   // "encontrou" é o retorno de calcularMelhorCaminho (1 = tem caminho, 0 = impossivel).

#endif