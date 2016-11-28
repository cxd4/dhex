CC=gcc
LDFLAGS=-L/usr/lib
CFLAGS=-I/usr/include -g
LIBS=-lncurses

OFILES=buffers.o input.o output.o machine_type.o main.o menu.o ui.o hexcalc.o search.o gpl.o configfile.o

all:	dhex

dhex:	$(OFILES)
	$(CC) $(LDFLAGS) -o $@ $(OFILES) $(LIBS)

.c.o:
	$(CC) $< -c -I. $(CFLAGS) $(OPTIONS)

clean:
	rm -f dhex $(OFILES)


