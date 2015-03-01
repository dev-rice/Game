PLATFORM := $(shell uname)

COMPILER := g++
COMPILER_FLAGS := -c -std=c++11 `sdl2-config --cflags` `freetype-config --cflags`

SRCDIR  := src
SRCEXT  := cpp
SOURCES := $(shell find $(SRCDIR) -type f -name *.$(SRCEXT))

OBJDIR  := obj
OBJECTS := $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o,$(SOURCES))

EXECUTABLE := game

MAC_LIBRARIES := -framework OpenGl -framework CoreFoundation -I/usr/local/include -lglew -lSOIL `sdl2-config --libs` `freetype-config --libs`
LINUX_LIBRARIES := -lGL -lGLEW -I /usr/lib/x86_64-linux-gnu/ -I /usr/local/include -lSOIL -lpthread `sdl2-config --cflags --libs`

# Try to auto detect the platform to build for
ifeq ($(PLATFORM),Darwin)
	LIBRARIES := $(MAC_LIBRARIES)
else ifeq ($(PLATFORM),Linux)
	LIBRARIES := $(LINUX_LIBRARIES)
endif

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(COMPILER) -I$(SRCDIR) $(OBJECTS) $(LIBRARIES) -o $@
	@ echo
	@ ./tools/buildcount.sh

$(OBJDIR)/%.o: $(SRCDIR)/%.$(SRCEXT)
	$(COMPILER) $(COMPILER_FLAGS) -I$(SRCDIR) $< -o $@

configure-linux:
	@ sudo apt-get install libglew-dev libglm-dev libsdl2-dev curl
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

clean:
	rm -f $(OBJDIR)/*.o
	rm -f $(OBJDIR)/particles/*.o
	rm -f $(OBJDIR)/core_ui/*.o
