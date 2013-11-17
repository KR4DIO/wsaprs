PROG=wsaprs
VER=0.0.1

CFLAGS=-std=gnu99 -Wall -Werror -Iwsrdr -I. -DVERSION='"${VER}"'

OBJS= ax25-tools/ax25/beacon.o wsaprs.o wsrdr/dfile.o wsrdr/chstream.o wsrdr/usbdrv.o wsrdr/wrecord.o wsrdr/header.o

LIBS=-lusb -lax25

wsaprs: ${OBJS}
	${CC} -o ${PROG} ${CFLAGS} ${OBJS} ${LIBS}

clean:
	rm -f ${PROG} ${OBJS}
