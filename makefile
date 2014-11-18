mac:
	g++ src/*.cpp -o glfw_test -framework OpenGl -framework CoreFoundation -I/usr/local/include -lglfw3 -lglew -lSOIL
linux:
	# no one knows