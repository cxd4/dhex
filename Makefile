SRC:=buffers.c configfile.c correlation.c gpl.c hexcalc.c input.c \
     machine_type.c  main.c  markers.c  menu.c  output.c  search.c  ui.c \
     hlmarkers.c
OBJ:=$(patsubst %.c,%.o,$(SRC))
DEP:=$(patsubst %.c,%.d,$(SRC))
CFLAGS:=-Wall -MMD $(shell pkgconf --cflags ncurses)
LDFLAGS:=$(shell pkgconf --libs ncurses)
TARGET=dhex

$(TARGET): $(OBJ)
	@echo linking $(TARGET)
	@gcc $(OBJ) $(LDFLAGS) -o $(TARGET)

%.o: %.c
	@echo compiling $<
	@gcc $(CFLAGS) -c $< -o $@

.PHONY: clean
clean:
	@rm -f $(OBJ) $(DEP) $(TARGET)

-include $(DEP)
