all:
	@ echo Please specify target platform
mac:
	@ ./tools/buildcount.sh
	@ rm -f res/models/*.mtl

	@ g++ src/*.cpp -std=c++11  -o glfw_test -framework OpenGl -framework CoreFoundation -I/usr/local/include -lglfw3 -lglew -lSOIL
linux:
	@ ./tools/buildcount.sh
	@ rm -f res/models/*.mtl

	@ g++ -std=c++11 src/*.cpp  -o glfw_test -lGL -lGLEW -I /usr/lib/x86_64-linux-gnu/ -lglfw -I /usr/local/include -lSOIL

discard:
	@ git clean -df
	@ git checkout -- .
