// profile.h
// Trevor Westphal

#ifndef Profile_h
#define Profile_h

#include <tuple>  
#include <stdio.h>
#include <string.h>

#include "debug.h"

class Profile {
public:
	static Profile* getInstance();

	int getFxaaLevel();
	bool getVsync();
	bool getWindowed();
	// Resolution TODO

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