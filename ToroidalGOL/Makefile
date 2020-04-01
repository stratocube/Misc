CC=gcc
CFLAGS=-c -Wall `pkg-config --cflags gl glu sdl2`
INCLUDE=-I/usr/local/{GL,SDL2}
LDFLAGS=`pkg-config --libs gl glu sdl2` -lglut -lm
SOURCES=gameoflife.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=life

all: $(SOURCES) $(EXECUTABLE)
   
$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) $(INCLUDE) $(LDFLAGS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f  $(OBJECTS) $(EXECUTABLE)

