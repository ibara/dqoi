# dqoi Makefile

CC ?=		cc
CFLAGS ?=	-g -O2
CFLAGS +=	-I/usr/X11R6/include
LDFLAGS =	-L/usr/X11R6/lib
LIBS =		-lX11

PROG =	dqoi
OBJS =	dqoi.o

all: ${OBJS}
	${CC} ${LDFLAGS} -o ${PROG} ${OBJS} ${LIBS}

clean:
	rm -f ${PROG} ${OBJS} ${PROG}.core
