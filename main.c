#include <stdio.h>
#include "caverna.h"
#include "pd.h"

int main(int argc, char *argv[]){
    Caverna c;
    PD pd;
    FILE *saida;
    int encontrou;

    if (argc != 2){
        fprintf(stderr, "Uso: %s <arquivo-da-caverna>\n", argv[0]);
        return 1;
    }

    if (!lerCaverna(&c, argv[1])){
        return 1;
    }

    if (!inicializarPD(&pd, c.linhas, c.colunas)){
        liberarCaverna(&c);
        return 1;
    }

    encontrou = calcularMelhorCaminho(&c, &pd);
    saida = fopen("resultado.txt", "w");
    if (saida == NULL){
        liberarPD(&pd);
        liberarCaverna(&c);
        return 1;
    }

    if (encontrou){
        escreverCaminho(&c, &pd, c.fimL, c.fimC, saida);
    }else{
        fprintf(saida, "impossível\n");
    }

    fclose(saida);
    liberarPD(&pd);
    liberarCaverna(&c);
    return 0;
}
