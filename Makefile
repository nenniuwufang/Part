.SUFFIXES: .cpp .o
CC=g++
CFLAGS=-c -std=c++0x -I /usr/local/include/fxt/ -O3 -DNDEBUG
LDFLAGS= -lfxt -L../fxt/lib/ -static
SOURCES=main.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=part
OTHERS=Makefile
ZIP=zip
ZIPFILE=part.zip

all: $(EXECUTABLE) $(ZIPFILE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

$(ZIPFILE): $(SOURCES) $(OTHERS)
	$(ZIP) $(ZIPFILE) $(SOURCES) $(OTHERS)

clean:
	rm -f $(OBJECTS) $(EXECUTABLE) $(ZIPFILE)
