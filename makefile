CC=gcc
CFLAGS=-c -Wall -Wextra -O3 -std=c89 -g

all: cmud
cmud: main.o data.o gui.o input.o
	$(CC) main.o data.o gui.o input.o -o cmud -lncursesw
main.o:
	 $(CC) $(CFLAGS) main.c -o main.o
data.o:
	 $(CC) $(CFLAGS) data.c -o data.o
gui.o:
	 $(CC) $(CFLAGS) gui.c -o gui.o
input.o:
	 $(CC) $(CFLAGS) input.c -o input.o
clean:
	rm -rf *.o cmud
