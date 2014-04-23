CC=g++
CFLAGS=-c -Wall
LDFLAGS=
SOURCES=$(shell ls src/*.cpp)
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=hello

-include -I /libtorrent/ *.mk $(bar)

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

.PHONY : clean
clean:
	-rm $(EXECUTABLE) $(OBJECTS)
