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
    return ts.tv_sec * 1000.0 + ts.tv_nsec / 1000000.0;
}
#endif

// Pergunta o nome do arquivo. Se digitar so o nome (ex: caverna1.txt), procura primeiro
// em Testes/ e depois em mapa/ (onde ficam as cavernas geradas).
// Se digitar com pasta (ex: Testes/caverna1.txt), usa do jeito que foi digitado.
void pedirArquivo(char *caminho){
    char nome[200];
    FILE *f;

    printf("Nome do arquivo da caverna (procura em Testes/ e mapa/): ");
    scanf("%199s", nome);

    if (strchr(nome, '/') || strchr(nome, '\\')){
        strcpy(caminho, nome);
        return;
    }

    sprintf(caminho, "Testes/%s", nome);
    f = fopen(caminho, "r");
    if (f != NULL){
        fclose(f);
        return;
    }

    sprintf(caminho, "mapa/%s", nome);
    f = fopen(caminho, "r");
    if (f != NULL){
        fclose(f);
        return;
    }

    printf("Arquivo '%s' nao encontrado em Testes/ nem em mapa/.\n", nome);
}

// Gera cavernas quadradas de varios tamanhos e mede o tempo da PD em cada uma.
// Serve de base para o grafico de tempo x tamanho da entrada.
// Retorna o tempo total gasto (geracao + leitura + PD).
double benchmark(){
    int tamanhos[] = {100, 200, 400, 800, 1600, 3200};
    int qtd = 6, repeticoes = 5;
    int i, j, r, n;
    double inicio, ini, soma, total;
    double tempos[6];
    Caverna c;
    PD pd;

    printf("\n--- Benchmark: tempo da PD x tamanho da caverna ---\n");
    printf("Gera cavernas NxN em mapa/benchmark.txt e mede a PD (media de %d execucoes).\n", repeticoes);

    inicio = agora();

    for (i = 0; i < qtd; i++){
        n = tamanhos[i];
        if (!gerarMapa(n, n, 1000000, 0, "benchmark.txt", 12345)){
            break;
        }
        if (!lerCaverna(&c, "mapa/benchmark.txt")){
            break;
        }

        // Forca a PD a percorrer a caverna inteira (de baixo/direita ate cima/esquerda),
        // assim o tempo depende so do tamanho NxN e nao de onde o I e o F foram sorteados.
        c.inicioL = n - 1;
        c.inicioC = n - 1;
        c.fimL = 0;
        c.fimC = 0;

        soma = 0;
        for (r = 0; r < repeticoes; r++){
            ini = agora();
            inicializarPD(&pd, n, n);
            calcularMelhorCaminho(&c, &pd);
            soma += agora() - ini;
            liberarPD(&pd);
        }

        tempos[i] = soma / repeticoes;
        liberarCaverna(&c);
    }

    total = agora() - inicio;

    // Tabela so no final, para nao misturar com as mensagens do gerador
    printf("\n%10s %14s %16s\n", "Tamanho", "Celulas", "Tempo PD (ms)");
    for (j = 0; j < i; j++){
        printf("%6dx%-4d %14d %16.4f\n", tamanhos[j], tamanhos[j], tamanhos[j] * tamanhos[j], tempos[j]);
    }
    printf("\nBenchmark concluido em %.4f ms\n", total);
    return total;
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
        printf("\n1 - Carregar caverna\n2 - Gerar caverna\n3 - Resolver\n4 - Benchmark (tempo x tamanho)\n0 - Sair\nOpcao: ");
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
            printf("\n--- Gerar caverna aleatoria ---\n");
            printf("As celulas sao sorteadas: vazias (0), monstros (-1 a -20) e pocoes (+1 a +20).\n");
            printf("O inicio (I) e a saida (F) tambem sao sorteados.\n\n");
            printf("Numero de linhas da caverna: ");
            scanf("%d", &linhas);
            printf("Numero de colunas da caverna: ");
            scanf("%d", &colunas);
            printf("Pontos de vida iniciais do estudante (ex: 50): ");
            scanf("%d", &hp);
            printf("Porcentagem de paredes, de 0 a 100 (0 = caverna sem paredes): ");
            scanf("%d", &perc);
            printf("Nome do arquivo a ser salvo na pasta mapa/ (ex: mapa1.txt): ");
            scanf("%199s", nome);

            t0 = agora();
            if (gerarMapa(linhas, colunas, hp, perc, nome, (unsigned int)time(NULL))){
                t1 = agora();
                tTotal += t1 - t0;
                printf("Geracao: %.4f ms\n", t1 - t0);
                printf("Para usar essa caverna, escolha a opcao 1 e digite: %s\n", nome);
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

        }else if (opcao == 4){
            tTotal += benchmark();
        }
    }while (opcao != 0);

    printf("\nTempo total de execucao medido: %.4f ms\n", tTotal);

    if (carregada){
        liberarCaverna(&c);
    }
    return 0;
}