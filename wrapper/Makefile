include ../include.mk
.PHONY: clean

CFLAGS += -DPIC -g -O0
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
