mkdir dhex_0.60
cp \
Makefile \
buffers.c \
buffers.h \
config.h \
configfile.c \
configfile.h \
gpl.c \
gpl.h \
gpl.txt \
hexcalc.c \
hexcalc.h \
input.c \
input.h \
machine_type.c \
machine_type.h \
main.c \
menu.c \
menu.h \
output.c \
output.h \
search.c \
search.h \
ui.c \
ui.h \
README.txt \
todo.txt \
mkrelease.sh \
dhex_0.60/
( cd dhex_0.60/ && make && make clean && cd .. && \
tar cvfz dhex_0.60.tar.gz dhex_0.60/ )
md5 dhex_0.60.tar.gz
