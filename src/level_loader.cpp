// level_loader.cpp
// Trevor Westphal

#include "level_loader.h"

Level::Level(){

}

void Level::loadLevel(const char * fileName){

    char buffer[128];

    FILE * ifile;
     ifile = fopen(fileName, "r");

    if(ifile == NULL){
        printf("Error opening file %s\n", fileName);
        return;
    
    }

     while(! feof(ifile)){
        if(fgets(buffer, 128, ifile) == NULL){
            // Can't read into buffer
            break;
        }

        printf("%s", buffer);

    }

}