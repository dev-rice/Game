// level_loader.cpp
// Trevor Westphal

#include "level_loader.h"

Level::Level(){

}

void Level::loadLevel(const char * fileName){

    char buffer[128];
    char objectFileName[20];
    char textureFileName[20];
    int objectIndex, diffIndex, specIndex, normIndex, emitIndex;
    float x, y, z, scale, x_rot, y_rot, z_rot;

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

        if(buffer[0] == 'm'){
            sscanf(buffer, "%*c %s", objectFileName);
            printf("Mesh: %s\n", objectFileName);
        }

        if(buffer[0] == 't'){
            sscanf(buffer, "%*c %s", textureFileName);
            printf("Texture: %s\n", textureFileName);
        }

        if(buffer[0] == 'd'){
            sscanf(buffer, "%*c %d %d %d %d %d %f %f %f %f %f %f %f", &objectIndex, &diffIndex, &specIndex, &normIndex, &emitIndex, &x, &y, &z, &scale, &x_rot, &y_rot, &z_rot);
            printf("Z: %f\n", z);
        }
    }

}