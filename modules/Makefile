include ../include.mk
.PHONY: clean


CFLAGS += -c -fpic
CPPFLAGS=$(CFLAGS)
LDFLAGS += -lz -lssl -lcrypto 

MODULE_SOURCES=CompressionModule.cpp EncryptionModule.cpp
SOURCES=$(MODULE_SOURCES) Preferences.cpp ContextualInfo.cpp

MODULE_HEADERS=DataPathModule.h CompressionModule.h EncryptionModule.h
HEADERS=$(MODULE_HEADERS) Preferences.h ContextualInfo.h

OBJS=$(SOURCES:.cpp=.o)
LIB=$(LIBDIR)/libSessionManagement.so

all: $(LIB)

%.o: %.cpp $(HEADERS)
	$(CC) $(CPPFLAGS) $<  -o $@

$(LIB): $(OBJS) 
	mkdir -p $(LIBDIR)
	$(LD) -shared -o $@ $(OBJS) $(LDFLAGS)

clean:
	-rm -f $(LIB)
	-rm -f *.o 
