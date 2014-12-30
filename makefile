CC := g++

SRCDIR := src
SRCEXT := cpp
SOURCES := $(shell find $(SRCDIR) -type f -name *.$(SRCEXT))

BIN := game

FRAMEWORKS := -framework OpenGl -framework CoreFoundation
MAC_LIB := -I/usr/local/include -lglfw3 -lglew -lSOIL

LINUX_LIB := -lGL -lGLEW -I /usr/lib/x86_64-linux-gnu/ -lglfw -I /usr/local/include -lSOIL

all:
	@ echo Please specify target platform
mac:
	@ ./tools/buildcount.sh
	@ rm -f res/models/*.mtl

	@ $(CC) -std=c++11 $(SOURCES) -o $(BIN) $(FRAMEWORKS) $(MAC_LIB)
linux:
	@ ./tools/buildcount.sh
	@ rm -f res/models/*.mtl

	@ $(CC) -std=c++11 $(SOURCES) -o $(BIN) $(LINUX_LIB)

discard:
	@ git clean -df
	@ git checkout -- .
