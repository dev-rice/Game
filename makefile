COMPILER := g++

OPTIONS :=

SRCDIR := ./src
SRCEXT := cpp
SOURCES := $(shell find $(SRCDIR) -type f -name *.$(SRCEXT))

BINARY_OUTPUT := game

FRAMEWORKS := -framework OpenGl -framework CoreFoundation
MAC_LIBRARIES := -I/usr/local/include -lglfw3 -lglew -lSOIL

LINUX_LIBRARIES := -lGL -lGLEW -I /usr/lib/x86_64-linux-gnu/ -lglfw -I /usr/local/include -lSOIL

all:
	@ echo Please specify target platform
mac:
	@ ./tools/buildcount.sh
	@ rm -f res/models/*.mtl

	@ $(COMPILER) $(OPTIONS) -std=c++11 $(SOURCES) -o $(BINARY_OUTPUT) $(FRAMEWORKS) $(MAC_LIBRARIES) -I$(SRCDIR)
linux:
	@ ./tools/buildcount.sh
	@ rm -f res/models/*.mtl

	@ $(COMPILER) $(OPTIONS) -std=c++11 $(SOURCES) -o $(BINARY_OUTPUT) $(LINUX_LIBRARIES) -I$(SRCDIR)

discard:
	@ git clean -df
	@ git checkout -- .
