// profile.h
// Trevor Westphal

#ifndef Profile_h
#define Profile_h

#include <tuple>  
#include <stdio.h>
#include <string.h>

#include "debug.h"
#include "window.h"

class Profile {
public:
	static Profile* getInstance();

	int getFxaaLevel();
	bool getVsync();
	bool getWindowed();
	// Resolution TODO

	void toggleVsync();

private:	
	int fxaa_level;
	bool vsync_on;
	bool windowed_on;
	int resolution;

	static Profile* instance;
	Profile();
	void loadSettings();
	std::tuple<char*, char*> split(char*, char);
};

#endif