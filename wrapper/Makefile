.PHONY: clean

LIBDIR=~/lib

CFLAGS=-c -Wall -Wextra -fPIC -DPIC -g -O0
LDFLAGS+=-lc -ldl
LIB=$(LIBDIR)/xwrap.so

ifdef DEBUG
	CFLAGS+=-ggdb -DDEBUG
endif

all: $(LIB)

$(LIB): xwrap.c
	$(CC) $(CFLAGS) $<
	$(LD) -shared -o $@ xwrap.o $(LDFLAGS) 

clean:
	-rm -f $(LIB)
	-rm -f *.o
