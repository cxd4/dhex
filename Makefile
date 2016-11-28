CC=		gcc
LDFLAGS=	-L/usr/lib	-L/usr/local/lib  	-L/usr/lib/ncurses	-L/usr/local/lib/ncurses
CPPFLAGS=	-I/usr/include	-I/usr/local/include	-I/usr/include/ncurses	-I/usr/local/include/ncurses
CFLAGS=		-O3 -Wall -std=c99 
#CFLAGS+= -ffunction-sections -fdata-sections
#LDFLAGS+= --gc-sections 
LIBS=		-lncurses
DESTDIR=	/usr/local/bin

OFILES=buffers.o input.o output.o machine_type.o main.o menu.o ui.o hexcalc.o search.o gpl.o configfile.o markers.o

all:	dhex

dhex:	$(OFILES)
	$(CC) $(LDFLAGS) -o $@ $(OFILES) $(LIBS)

install:all
	strip dhex
	cp dhex $(DESTDIR)
	

.c.o:
	$(CC) $< -c -I. $(CPPFLAGS) $(CFLAGS) $(OPTIONS)

clean:
	rm -f dhex $(OFILES)


