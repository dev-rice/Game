#include "texture_set.h"

TextureSet::TextureSet(GLuint diff, GLuint spec, GLuint norm, GLuint emit){
    printf("Creating Texture Set with:\n");
    printf("  diffuse:  %d\n", diff);
    printf("  specular: %d\n", spec);
    printf("  normal:   %d\n", norm);
    printf("  emissive: %d\n\n", emit);

    GLuint pink = TextureLoader::getPink();
    GLuint alpha = TextureLoader::getAlpha();

    printf("Pink:  %d\n", pink);
    printf("Alpha: %d\n", alpha);

    if (diff == 0){
        printf("diffuse is zero, setting ");
        this->diffuse = pink;
        this->specular = pink;
        this->normal = pink;
        this->emissive = pink;
    }
    else {
        printf("diffuse is set");
        this->diffuse = diff;
        this->specular = spec;
        this->normal = norm;
        this->emissive = emit;
        if (spec == 0){
            this->specular = alpha;
        }
        if (norm == 0){
            this->normal = alpha;
        }
        if (emissive == 0){
            this->emissive = alpha;
        }
    }

    // printf("Texture Set:\n");
    // printf("  diffuse  = %d\n", diffuse);
    // printf("  specular = %d\n", specular);
    // printf("  normal   = %d\n", normal);
    // printf("  emissive = %d\n\n", emissive);

    
}
