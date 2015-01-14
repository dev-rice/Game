// profile.cpp
// Trevor Westphal

#include "profile.h"

Profile* Profile::instance;

Profile* Profile::getInstance(){
	if(instance){
		return instance;
	} else {
		instance = new Profile();
		return instance;
	}
}

Profile::Profile(){
	loadSettings();
}

int Profile::getFxaaLevel(){
	return fxaa_level;
}

bool Profile::getVsync(){
	return vsync_on;
}

void Profile::toggleVsync(){
	vsync_on = !vsync_on;
	Window::getInstance()->setVsync(vsync_on);
}


bool Profile::getWindowed(){
	return windowed_on;
}

void Profile::loadSettings(){
	char buffer[128];

    FILE * ifile;
    ifile = fopen("settings/settings.conf", "r");

    if(ifile == NULL){
        Debug::error("Error opening configuration file.\n");
        return;

    }

     while(! feof(ifile)){
     	if(fgets(buffer, 128, ifile) == NULL){
            // Can't read into buffer
            break;
        }
        
        // skip commented lines
        if(buffer[0] != '#'){
        	std::tuple<char*, char*> arguments = split(buffer, '=');

        	char* keyword =  std::get<0>(arguments);
        	char* value = std::get<1>(arguments);

        	if(strcmp(keyword, "fxaa") == 0){
        		fxaa_level = atoi(value);
        	} else if(strcmp(keyword, "vsync") == 0){
        		vsync_on = (strcmp(value, "true") == 0);
        	} else if(strcmp(keyword, "windowed") == 0){
        		windowed_on = (strcmp(value, "true") == 0);
        	} else if(strcmp(keyword, "resolution") == 0){
        		resolution = atoi(value);
        	}
        }
    }
}

std::tuple<char*, char*> Profile::split(char* str, char key){
	bool found_key = false;

	int str_count = 0;
	int str_2_count = 0;

	char* sub_str_1 = new char[32];
	char* sub_str_2 = new char[32];

	for(int i = 0; str[i] != 0; i++){
		if(str[i] == key){
			found_key = true;
			i++;
		}
		if(!found_key){
			str_count++;
			sub_str_1[i] = str[i];
		} else {
			sub_str_2[str_2_count] = str[i];
			str_2_count++;
		}
	}

	sub_str_1[str_count] = 0;
	sub_str_2[str_2_count-1] = 0;

	return std::make_tuple(sub_str_1, sub_str_2);
}