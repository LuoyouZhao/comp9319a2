all:bwtsearch avl

bwtsearch: bwtsearch.c
	gcc -lm bwtsearch.c -o bwtsearch
avl: avl.c
	gcc -lm avl.c -o avl
clean:
	rm bwtsearch avl