CC=g++
CFLAGS=-std=c++14 -g
LDFLAGS=-larmadillo -lSDL2 -lSDL2_image

SOURCES=main.cpp Map.cpp Tile.cpp Player.cpp Raycaster.cpp
OBJECTS=$(SOURCES:.cpp=.o)
DEPS=Map.h Tile.h Player.h Raycaster.h
TARGET=raycaster

$(TARGET) : $(OBJECTS) 
	$(CC) $(LDFLAGS) -o $@ $^

$(OBJECTS): $(SOURCES) $(DEPS)
	$(CC) $(CFLAGS) -c $(SOURCES)

.PHONY: clean

clean:
	@rm *.o $(TARGET)
