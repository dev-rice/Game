mac:
	g++ src/*.cpp -o glfw_test -framework OpenGl -framework CoreFoundation -I/usr/local/include -lglfw3 -lglew -lSOIL
linux:
	g++ -std=c++11 src/*.cpp -o glfw_test -lGL -lGLEW -I /usr/lib/x86_64-linux-gnu/ -lglfw -I /usr/local/include -lSOIL	