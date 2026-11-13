#ifndef CAVERNA_H
#define CAVERNA_H
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