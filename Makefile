teste: main.o caverna.o pd.o
	gcc main.o caverna.o pd.o -o teste

main.o: main.c Headers/caverna.h Headers/pd.h
	gcc -Wall -IHeaders -c main.c -o main.o

caverna.o: Codigos-fonte/caverna.c Headers/caverna.h
	gcc -Wall -IHeaders -c Codigos-fonte/caverna.c -o caverna.o

pd.o: Codigos-fonte/pd.c Headers/pd.h Headers/caverna.h
	gcc -Wall -IHeaders -c Codigos-fonte/pd.c -o pd.o

clean:
	rm -f *.o teste

all: teste