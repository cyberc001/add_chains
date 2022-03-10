CC_FLAGS=-g -Wall
CC=gcc $(CC_FLAGS)
OCC=$(CC) -c

add_chains: add_algos.o bin_utils.o chain_utils.o main.o
	$(CC) -o $@ $^

clean:
	-rm add_chains
	-rm *.o

add_algos.o: add_algos.c add_algos.h chain_utils.h bin_utils.h
	$(OCC) -o $@ $<
bin_utils.o: bin_utils.c bin_utils.h
	$(OCC) -o $@ $<
chain_utils.o: chain_utils.c chain_utils.h
	$(OCC) -o $@ $<
main.o: main.c add_algos.h chain_utils.h
	$(OCC) -o $@ $<
