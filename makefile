PLATFORM := $(shell uname)

COMPILER := g++

OPTIONS :=

SRCDIR := ./src
SRCEXT := cpp
SOURCES := $(shell find $(SRCDIR) -type f -name *.$(SRCEXT))

BINARY_OUTPUT := game

FRAMEWORKS := -framework OpenGl -framework CoreFoundation
MAC_LIBRARIES := -I/usr/local/include -lglfw3 -lglew -lSOIL

LINUX_LIBRARIES := -lGL -lGLEW -I /usr/lib/x86_64-linux-gnu/ -lglfw -I /usr/local/include -lSOIL -lpthread
LINUX_LIBRARIES_ALT := -lglfw3 -lX11 -lXxf86vm -lXrandr -lpthread -lXi -lrt -lXinerama -lXcursor -lGL -lGLEW -I /usr/local/include -lSOIL

all:
# Try to auto detect the platform to build for
ifeq ($(PLATFORM),Darwin)
	@ echo Building for Mac OSX.
	@ make mac
else ifeq ($(PLATFORM),Linux)
	@ echo Building for Linux.
	@ make linux
else
	@ echo Unknown platform '$(PLATFORM)'
endif

mac:
	@ ./tools/buildcount.sh
	@ rm -f res/models/*.mtl

	@ $(COMPILER) $(OPTIONS) -std=c++11 $(SOURCES) -o $(BINARY_OUTPUT) $(FRAMEWORKS) $(MAC_LIBRARIES) -I$(SRCDIR)
linux:
	@ ./tools/buildcount.sh
	@ rm -f res/models/*.mtl

	@ $(COMPILER) $(OPTIONS) -std=c++11 $(SOURCES) -o $(BINARY_OUTPUT) $(LINUX_LIBRARIES) -I$(SRCDIR)

linux-alt:
	@ ./tools/buildcount.sh
	@ rm -f res/models/*.mtl

	@ $(COMPILER) $(OPTIONS) -std=c++11 $(SOURCES) -o $(BINARY_OUTPUT) $(LINUX_LIBRARIES_ALT) -I$(SRCDIR)

configure-linux:
	@ sudo apt-get install libglew-dev libglm-dev libglfw3-dev curl
	@ wget http://www.lonesock.net/files/soil.zip
	@ unzip soil.zip -d soil
	@ rm soil.zip
	@ rm soil/Simple\ OpenGL\ Image\ Library/lib/libSOIL.a
	@ mkdir soil/Simple\ OpenGL\ Image\ Library/projects/makefile/obj
	@ cd soil/Simple\ OpenGL\ Image\ Library/projects/makefile && make && sudo make install
	@ rm -r soil

discard:
	@ git clean -df
	@ git checkout -- .
