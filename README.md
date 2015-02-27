# Game

# Dependencies
GLEW

SFML 2.2

GLM

SOIL

## Ubuntu installation instructions
Run

```sudo apt-get install libglew-dev libglm-dev libsfml-dev curl```

```wget http://www.lonesock.net/files/soil.zip```

cd Into the SOIL directory and type

```rm lib/libSOIL.a```

cd into ```projects/makefile/```

type ```mkdir obj```

run ``` make && sudo make install```

OR

run the make command ```make configure-linux```
