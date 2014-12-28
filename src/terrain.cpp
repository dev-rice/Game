#include "terrain.h"

Terrain::Terrain(Mesh* mesh, GLuint shader_program) : Drawable(mesh, shader_program){

}

Terrain::Terrain(Mesh* mesh, GLuint shader_program, glm::vec3 position, GLfloat scale): Drawable(mesh, shader_program, position, scale) {

}

Terrain::Terrain(Mesh* mesh, GLuint shader_program, glm::vec3 position, glm::vec3 rotation, GLfloat scale): Drawable(mesh, shader_program, position, rotation, scale) {

}

Terrain::Terrain(Mesh* mesh, GLuint shader_program, glm::vec3 position, GLfloat scale, const char* heightMapFileName): Drawable(mesh, shader_program, position, scale) {
    // This is where generate the new mesh and override the one passed in by
    // the constructor. This is to save space in the game files, so we don't have a terrain mesh

    // First, we load the actual image data, and post to debug
    image = SOIL_load_image(heightMapFileName, &width, &height, 0, SOIL_LOAD_RGBA);

    if(image){
        Debug::info("Loaded heightmap \"%s\" into memory.\n", heightMapFileName);
    } else {
        Debug::error("Could not load heightmap \"%s\" into memory.\n", heightMapFileName);
    }

    // Now, we must generate a mesh from the data in the heightmap. We use 
    // 1-BASED INDEXING because of the need for an offset
    float mapHeight;
    for(int y = 0; y < height; ++y){
        for(int x = 0; x < width; ++x){
            mapHeight = getHeight(x, y);
            printf("%f\t", mapHeight);
        }
        printf("\n");
    }
}

float Terrain::getHeight(int x, int y){
    int red = image[(y*width + x)*4 + 0];
    int grn = image[(y*width + x)*4 + 1];
    int blu = image[(y*width + x)*4 + 2];

    return float(red + grn + blu)/3.0f;
}

void Terrain::attachTextureSet(TextureSet texture_set){
    // Prepare the locations for textures to load into and give this drawable some textures. These are specific
    // to Drawable for now but will later be moved down to the child classes such that they can specify different
    // amounts and types of textures to use.

    // When we set this uniform we tell the shader that "diffuse_texture" will be loaded from the 0th texture, and so on.
    // The actual images these numbers point to are specified later in bindTextures().
    glUniform1i(glGetUniformLocation(shader_program, "diffuse_texture"), 0);
    glUniform1i(glGetUniformLocation(shader_program, "heightmap"), 1);

    this->texture_set = new TextureSet(texture_set);

}

void Terrain::bindTextures(){
    // Put each texture into the correct location for this Drawable. GL_TEXTURE0-3
    // correspond to the uniforms set in attachTextureSet(). This is where we actually
    // tell the graphics card which textures to use.
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture_set->diffuse);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture_set->specular);

}

void Terrain::updateUniformData(){
    // Set the scale, this is not really going to be a thing, probably
    // ^ It's definitely a thing
    glUniform1f(glGetUniformLocation(shader_program, "scale"), scale);

    // Tell the shader the current time
    glUniform1f(glGetUniformLocation(shader_program, "time"), (float)glfwGetTime());

}
