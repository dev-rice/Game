# Details of code:
#
# echo $$(( $$(cat buildcount.txt) + 1 )) > buildcount.txt
# This reads in from buildcount.txt, increments, and saves
#
# echo Build number $$(cat buildcount.txt)
# This prints out the build number
#
# rm -f res/models/*.mtl
# This gets rid of any stray material (*.mtl) files in resources
#
# git clean -df
# git checkout -- .
#This removes all uncommitted changes, reverting to the last commit

all:
	@ echo Please specify target platform
mac:
	@ echo $$(( $$(cat buildcount.txt) + 1 )) > buildcount.txt
	@ echo Build number $$(cat buildcount.txt)
	@ curl -s http://192.168.1.53:8080
	@ rm -f res/models/*.mtl

	@ g++ src/*.cpp -std=c++11  -o glfw_test -framework OpenGl -framework CoreFoundation -I/usr/local/include -lglfw3 -lglew -lSOIL
linux:
	@ echo $$(( $$(cat buildcount.txt) + 1 )) > buildcount.txt
	@ echo Build number $$(cat buildcount.txt)
	@ curl -s http://192.168.1.53:8080
	@ rm -f res/models/*.mtl

	@ g++ -std=c++11 src/*.cpp  -o glfw_test -lGL -lGLEW -I /usr/lib/x86_64-linux-gnu/ -lglfw -I /usr/local/include -lSOIL	

discard:
	@ git clean -df
	@ git checkout -- .
