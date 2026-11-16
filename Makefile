bin/caverna: obj/main.o obj/caverna.o obj/pd.o obj/gerador.o
	mkdir -p bin
	gcc obj/main.o obj/caverna.o obj/pd.o obj/gerador.o -o bin/caverna

obj/main.o: main.c Headers/caverna.h Headers/pd.h Headers/gerador.h
	mkdir -p obj
	gcc -Wall -IHeaders -c main.c -o obj/main.o

obj/caverna.o: Codigos-fonte/caverna.c Headers/caverna.h
	mkdir -p obj
	gcc -Wall -IHeaders -c Codigos-fonte/caverna.c -o obj/caverna.o

obj/pd.o: Codigos-fonte/pd.c Headers/pd.h Headers/caverna.h
	mkdir -p obj
	gcc -Wall -IHeaders -c Codigos-fonte/pd.c -o obj/pd.o

obj/gerador.o: Codigos-fonte/gerador.c Headers/gerador.h Headers/caverna.h
	mkdir -p obj
	gcc -Wall -IHeaders -c Codigos-fonte/gerador.c -o obj/gerador.o

run: bin/caverna
	./bin/caverna testes/caverna5.txt

clean:
	rm -f obj/*.o bin/caverna

all: bin/caverna