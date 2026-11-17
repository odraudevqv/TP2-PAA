#include <stdio.h>
#include <string.h>
#include <time.h>
#include "caverna.h"
#include "pd.h"
#include "gerador.h"

// Tempo atual em milissegundos (relogio de alta resolucao)
#ifdef _WIN32
#include <windows.h>
double agora(){
    LARGE_INTEGER freq, cont;
    QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&cont);
    return (double)cont.QuadPart * 1000.0 / (double)freq.QuadPart;
}
#else
double agora(){
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec * 1000.0 + ts.tv_nsec / 1000000.0
}
#endif


void pedirArquivo(char *caminho){
    char nome[200];
    printf("Arquivo da caverna (ex: caverna1.txt ou mapa/t.txt): ");
    scanf("%199s", nome);
    if (strchr(nome, '/') || strchr(nome, '\\')){
        strcpy(caminho, nome);
    }else{
        sprintf(caminho, "Testes/%s", nome);
    }
}

int main(int argc, char *argv[]){
    Caverna c;
    PD pd;
    FILE *saida;
    char caminho[260], nome[200];
    int opcao, carregada, encontrou;
    int linhas, colunas, hp, perc;
    double tLeitura, tPD, tTotal = 0;
    double t0, t1;

    carregada = 0;
    tLeitura = 0;

    // Se o arquivo vier pela linha de comando, ja carrega. Senao, comeca direto no menu.
    if (argc == 2){
        t0 = agora();
        carregada = lerCaverna(&c, argv[1]);
        tLeitura = agora() - t0;
        tTotal += tLeitura;
        if (carregada){
            printf("Caverna carregada (%.4f ms)\n", tLeitura);
        }
    }

    do{
        printf("\n1 - Carregar caverna\n2 - Gerar caverna\n3 - Resolver\n0 - Sair\nOpcao: ");
        scanf("%d", &opcao);

        if (opcao == 1){
            pedirArquivo(caminho);
            if (carregada){
                liberarCaverna(&c);
            }
            t0 = agora();
            carregada = lerCaverna(&c, caminho);
            tLeitura = agora() - t0;
            tTotal += tLeitura;
            if (carregada){
                printf("Caverna carregada (%.4f ms)\n", tLeitura);
            }

        }else if (opcao == 2){
            printf("Linhas colunas HP porcentagem_de_paredes: ");
            scanf("%d %d %d %d", &linhas, &colunas, &hp, &perc);
            printf("Nome do arquivo (ex: mapa1.txt): ");
            scanf("%199s", nome);
            t0 = agora();
            if (gerarMapa(linhas, colunas, hp, perc, nome, (unsigned int)time(NULL))){
                t1 = agora();
                tTotal += t1 - t0;
                printf("Geracao: %.4f ms (arquivo em mapa/%s)\n", t1 - t0, nome);
            }

        }else if (opcao == 3){
            if (!carregada || c.inicioL < 0 || c.fimL < 0){
                printf("Carregue uma caverna valida primeiro (opcao 1).\n");
                continue;
            }

            t0 = agora();
            inicializarPD(&pd, c.linhas, c.colunas);
            encontrou = calcularMelhorCaminho(&c, &pd);

            saida = fopen("resultado.txt", "w");
            if (encontrou){
                escreverCaminho(&c, &pd, c.fimL, c.fimC, saida);
            }else{
                fprintf(saida, "impossível\n");
            }
            fclose(saida);
            t1 = agora();
            tPD = t1 - t0;
            tTotal += tPD;

            if (encontrou){
                printf("\nCaminho (de I ate F):\n");
                escreverCaminho(&c, &pd, c.fimL, c.fimC, stdout);
                printf("HP final: %d\n", pd.tabela[c.fimL][c.fimC]);
            }else{
                printf("\nImpossivel escapar.\n");
            }
            printf("Resultado gravado em resultado.txt\n");
            printf("Tempo leitura: %.4f ms | Tempo PD + gravacao: %.4f ms\n", tLeitura, tPD);

            liberarPD(&pd);
        }
    }while (opcao != 0);

    printf("\nTempo total de execucao medido: %.4f ms\n", tTotal);

    if (carregada){
        liberarCaverna(&c);
    }
    return 0;
}