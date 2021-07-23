all:bwtsearch

bwtsearch: bwtsearch.c
	gcc -lm bwtsearch.c -o bwtsearch