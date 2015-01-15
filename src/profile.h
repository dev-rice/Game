// profile.h
// Trevor Westphal

#ifndef Profile_h
#define Profile_h

#include <tuple>
#include <map>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "debug.h"
#include "window.h"

class Profile {
public:
	static Profile* getInstance();

	int getFxaaLevel();
	bool getVsync();
	bool getWindowed();
	int getWindowHeight();
	int getWindowWidth();

	void toggleVsync();

private:
	int fxaa_level;
	bool vsync_on;
	bool windowed_on;
	int resolution_index;
	std::map<int, std::tuple<int, int>> resolution_map;

	static Profile* instance;
	Profile();
	void loadSettings();
	std::tuple<char*, char*> split(char*, char);
};

#endif
