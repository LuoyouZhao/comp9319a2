all:bwtsearch
bwtsearch: bwtsearch.c
	gcc -c avl.c
	gcc -c bwtsearch.c
	gcc -o bwtsearch bwtsearch.o avl.o
# clean:
# 	rm bwtsearch avl