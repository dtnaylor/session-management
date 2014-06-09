include ../include.mk
.PHONY: clean

CC=g++
CFLAGS += -DPIC -g -O0 -I../manager/ -I../modules/ -std=c++0x
LIB=$(LIBDIR)/xwrap.so

ifdef DEBUG
	CFLAGS+=-ggdb -DDEBUG
endif

all: $(LIB)

$(LIB): xwrap.cpp ../manager/Manager.cpp
	mkdir -p $(LIBDIR)
	$(CC) $(CFLAGS) $<
	$(CC) $(CFLAGS) ../manager/Manager.cpp
	$(CC) -shared -o $@ xwrap.o ../manager/Manager.o $(LDFLAGS) 

clean:
	-rm -f $(LIB)
	-rm -f *.o
