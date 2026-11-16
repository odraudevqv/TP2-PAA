#ifndef GERADOR_H
#define GERADOR_H

// Gera uma caverna aleatoria e salva no arquivo mapa/<nomeArquivo>.
// A pasta "mapa" precisa existir (a funcao nao cria a pasta).
// O nome do arquivo deve ser passado completo, com o .txt (ex: "caverna5.txt").
// Para gerar um mapa diferente a cada vez, passe time(NULL) como semente.
// Retorna 1 se deu certo e 0 se deu algum erro.
int gerarMapa(int linhas, int colunas, int hpInicial, int percParedes, char *nomeArquivo, unsigned int semente);

#endif
