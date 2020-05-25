all: victim.s preload.c transactionBegin.c
	clang -c victim.s -o victim.o
	clang -c preload.c -o preload.o
	clang -c -mrtm transactionBegin.c -o transactionBegin.o
	clang preload.o transactionBegin.o victim.o -o victim