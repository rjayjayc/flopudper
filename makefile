CC = gcc -std=gnu99
CFLAGS = -Wall -g -I.
DEPS = shell.h boot.h Fat12Boot.h entry.h
OBJ = Project2.c shell.c boot.c serv.c myclient.c

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

flop: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean

clean:
	rm -f *.o *~ flop
