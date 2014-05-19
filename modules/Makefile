include ../include.mk
.PHONY: clean


CFLAGS += -c -fpic
CPPFLAGS=$(CFLAGS)
LDFLAGS += -lz -lssl -lcrypto 

MODULE_SOURCES=CompressionModule.cpp
SOURCES=$(MODULE_SOURCES) Preferences.cpp ContextualInfo.cpp

MODULE_HEADERS=DataPathModule.h CompressionModule.h
HEADERS=$(MODULE_HEADERS) Preferences.h ContextualInfo.h

OBJS=$(SOURCES:.cpp=.o)
LIB=$(LIBDIR)/libSessionManagement.so

all: $(LIB)

%.o: %.cpp $(HEADERS)
	$(CC) $(CPPFLAGS) $<  -o $@

$(LIB): $(OBJS) 
	$(LD) -shared -o $@ $(OBJS) $(LDFLAGS)

clean:
	-rm -f $(LIB)
	-rm -f *.o 
