CC=g++
CFLAGS=-c -Wall

SOURCES=main.cpp

OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=Compiler

all: $(SOURCES) $(EXECUTABLE) clean

clean:
	rm -f -rf *.o

$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@
