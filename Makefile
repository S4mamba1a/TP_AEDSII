all:
	gcc main.c Codes/arquivo.c Codes/busca.c Codes/hash.c Codes/patricia.c -IHeaders -o programa -lm

clean:
	rm -f programa