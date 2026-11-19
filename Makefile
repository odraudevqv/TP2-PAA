# Funciona no Windows (mingw32-make) e no Linux (make)
ifeq ($(OS),Windows_NT)
    EXECUTAVEL = caverna.exe
    RODAR = caverna.exe
    CRIAR_MAPA = if not exist mapa mkdir mapa
    LIMPAR = if exist caverna.exe del caverna.exe
else
    EXECUTAVEL = caverna
    RODAR = ./caverna
    CRIAR_MAPA = mkdir -p mapa
    LIMPAR = rm -f caverna
endif

FONTES = main.c Codigos-fonte/caverna.c Codigos-fonte/pd.c Codigos-fonte/gerador.c Codigos-fonte/visual.c

$(EXECUTAVEL): $(FONTES) Headers/caverna.h Headers/pd.h Headers/gerador.h Headers/visual.h
	gcc -Wall -IHeaders $(FONTES) -o $(EXECUTAVEL)

# Compila e executa (o arquivo da caverna e digitado com o programa rodando)
run: $(EXECUTAVEL)
	@$(CRIAR_MAPA)
	@$(RODAR)

clean:
	$(LIMPAR)

all: $(EXECUTAVEL)