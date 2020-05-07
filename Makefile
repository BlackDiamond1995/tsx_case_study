all: evict_demo.c
	gcc evict_demo.c -o evict_demo -lpthread

clean:
	rm evict_demo