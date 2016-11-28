mkdir dhex_0.62
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
markers.c \
markers.h \
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
dhex_0.62/
( cd dhex_0.62/ && make && make clean && cd .. && \
tar cvfz dhex_0.62.tar.gz dhex_0.62/ )
cp dhex_0.62.tar.gz dhex_latest.tar.gz
md5 dhex_0.62.tar.gz
md5 dhex_latest.tar.gz

