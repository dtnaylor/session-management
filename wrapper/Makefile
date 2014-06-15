include ../include.mk
.PHONY: clean

CC=g++
CFLAGS += -DPIC -g -O0 -I../manager/ -I../modules/ -I./ -std=c++0x
LIB=$(LIBDIR)/xwrap.so

ifdef DEBUG
	CFLAGS+=-ggdb -DDEBUG
endif

all: $(LIB)

$(LIB): xwrap.cpp ../manager/Manager.cpp ../modules/EncryptionModule.cpp ../modules/CompressionModule.cpp
	mkdir -p $(LIBDIR)
	$(CC) $(CFLAGS) $<
	$(CC) $(CFLAGS) ../manager/Manager.cpp
	$(CC) $(CFLAGS) ../modules/EncryptionModule.cpp
	$(CC) $(CFLAGS) ../modules/CompressionModule.cpp
	$(CC) -shared -o $@ xwrap.o Manager.o EncryptionModule.o CompressionModule.o -lssl -lcrypto $(LDFLAGS) 

clean:
	-rm -f $(LIB)
	-rm -f *.o
