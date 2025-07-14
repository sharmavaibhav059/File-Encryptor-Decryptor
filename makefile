flags=-O2 -Wall -std=c2x
ldflags=

.PHONY: all clean

all: clean fileEncry

all: clean fsd

fsd: fsd.o arcfour.o
	cc $(flags) fsd.o arcfour.o -o fsd

fsd.o: fsd.c fsd.h 
	cc $(flags) -c $<

fileEncry: fileEncry.o arcfour.o
	cc $(flags) $^ -o $@ $(ldflags)

fileEncry.o: fileEncry.c fileEncry.h 
	cc $(flags) -c $<

arcfour.o: arcfour.c arcfour.h
	cc $(flags) -c arcfour.c

clean:
	rm -f *.o fileEncry fsd
