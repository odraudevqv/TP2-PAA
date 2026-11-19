#comandos recorrentemente executados
EXEC = ./bin/tp
CC = gcc
HEADER = -I./Headers

#variaveis com os nomes das pastas
SRC = ./Codigos-fonte
OBJ = obj
BIN = bin
TP = ./

FONTES = $(wildcard $(SRC)/*.c)
OBJETOS = $(patsubst $(SRC)/%.c,$(OBJ)/%.o,$(FONTES))

#no Windows o executavel precisa de .exe
ifeq ($(OS),Windows_NT)
	EXEC := $(EXEC).exe
endif

#unico ponto que depende do shell: a limpeza
ifeq ($(findstring sh,$(SHELL)),sh)
	LIMPAR = rm -f obj/*.o bin/*
else
	LIMPAR = del /Q obj\*.o bin\*.exe
endif

all: $(EXEC)

#compila o main e linka com todos os objetos
$(EXEC): $(OBJETOS) $(TP)/main.c | $(BIN)
	$(CC) $(TP)/main.c $(OBJETOS) $(HEADER) -o $(EXEC) -lm

#cada .c vira um .o (as pastas sao criadas antes, se nao existirem)
$(OBJ)/%.o: $(SRC)/%.c | $(OBJ)
	$(CC) -c $< $(HEADER) -o $@

#o make so executa estas regras se a pasta nao existir
$(OBJ) $(BIN):
	mkdir $@

run:
	$(EXEC)

clean:
	$(LIMPAR)

.PHONY: all executar limpar