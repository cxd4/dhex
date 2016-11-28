CC=		gcc
LDFLAGS=	-L/usr/lib	-L/usr/local/lib  	-L/usr/lib/ncurses	-L/usr/local/lib/ncurses
CPPFLAGS=	-I/usr/include	-I/usr/local/include	-I/usr/include/ncurses	-I/usr/local/include/ncurses
CFLAGS=		-O3 -Wall -std=c99 
#CFLAGS+= -ffunction-sections -fdata-sections
#LDFLAGS+= --gc-sections 
LIBS=		-lncurses
DESTDIR=	/usr/local/

OFILES=buffers.o \
	configfile.o \
	correlation.o \
	gpl.o \
	hexcalc.o \
	input.o \
	machine_type.o \
	main.o \
	markers.o \
	menu.o \
	output.o \
	search.o \
	ui.o

all:	dhex

dhex:	$(OFILES)
	$(CC) $(LDFLAGS) -o $@ $(OFILES) $(LIBS)

install:all
	strip dhex
	cp dhex $(DESTDIR)/bin
	cp dhex.1 $(DESTDIR)/man/man1
	cp dhexrc.5 $(DESTDIR)/man/man5
	cp dhex_markers.5 $(DESTDIR)/man/man5
	cp dhex_searchlog.5 $(DESTDIR)/man/man5

	

.c.o:
	$(CC) $< -c -I. $(CPPFLAGS) $(CFLAGS) $(OPTIONS)

clean:
	rm -f dhex $(OFILES)


