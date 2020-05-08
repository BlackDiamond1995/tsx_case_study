all: evict_demo.c
	gcc evict_demo.c -o evict_demo -lpthread

clean:
	rm -f evict_demo targetfile lockfile2G
