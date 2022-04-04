# dqoi Makefile

PREFIX ?=	/usr/local
MANDIR ?=	${PREFIX}/man

CC ?=		cc
CFLAGS ?=	-g -O2
CFLAGS +=	-I/usr/X11R6/include
LDFLAGS =	-L/usr/X11R6/lib
LIBS =		-lX11

PROG =	dqoi
OBJS =	dqoi.o

all: ${OBJS}
	${CC} ${LDFLAGS} -o ${PROG} ${OBJS} ${LIBS}

install:
	install -d 755 ${DESTDIR}${PREFIX}/bin
	install -d 755 ${DESTDIR}${MANDIR}/man1
	install -c -S -s -m 755 ${PROG} ${DESTDIR}${PREFIX}/bin
	install -c -m 644 ${PROG}.1 ${DESTDIR}${MANDIR}/man1

clean:
	rm -f ${PROG} ${OBJS} ${PROG}.core
