#include "terrain.h"

Terrain::Terrain(GLuint shader_program, std::string heightmap_filename) : Drawable() {
    // This is where generate the new mesh and override the one passed in by
    // the constructor. This is to save space in the game files, so we don't have a terrain mesh

    // First, we load the actual image data, and post to debug
    image = SOIL_load_image(heightmap_filename.c_str(), &image_width, &image_height, 0, SOIL_LOAD_RGBA);

    if(image){
        Debug::info("Loaded heightmap \"%s\" into memory.\n", heightmap_filename.c_str());
    } else {
        Debug::error("Could not load heightmap \"%s\" into memory.\n", heightmap_filename.c_str());
    }

    mesh = generateMesh();

    Drawable::load(mesh, shader_program, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), 1.0f);

}

Mesh* Terrain::generateMesh(){
    std::vector<GLfloat> vertices_vector;
    std::vector<GLuint> faces_vector;

    float start_x = -image_width / 2.0;
    float start_z = -image_height / 2.0;

    float u_inc = 1.0 / (float)image_width;
    float v_inc = 1.0 / (float)image_height;

    // Now, we must generate a mesh from the data in the heightmap. We use
    // 1-BASED INDEXING because of the need for an offset
    float map_height;
    glm::vec3 pos;
    for(int y = 0; y < image_height; ++y){
        for(int x = 0; x < image_width; ++x){
            map_height = getHeight(x, y) / 255.0;
            pos = glm::vec3(start_x + x, map_height, start_z + y);
            printf("%f\t", map_height);
            // Position
            vertices_vector.push_back(pos.x);
            vertices_vector.push_back(pos.y);
            vertices_vector.push_back(pos.z);

            // Normal calculations need to be done
            vertices_vector.push_back(0.0f);
            vertices_vector.push_back(1.0f);
            vertices_vector.push_back(0.0f);

            // Texcoord
            vertices_vector.push_back(u_inc * x);
            vertices_vector.push_back(v_inc * y);

        }
        printf("\n");
    }

    for (int y = 0; y < image_height - 1; ++y){
        for (int x = 0; x < image_width - 1; ++x){
            int y_index = (y * 4);
            faces_vector.push_back(x + y_index);
            faces_vector.push_back(x + y_index + 4);
            faces_vector.push_back(x + y_index + 1);

            faces_vector.push_back(x + y_index + 1);
            faces_vector.push_back(x + y_index + 4);
            faces_vector.push_back(x + y_index + 5);
        }
    }

    Mesh* ground = new Mesh(vertices_vector, faces_vector);
    return ground;
}

float Terrain::getHeight(int x, int y){
    int red = image[(y*image_width + x)*4 + 0];
    int grn = image[(y*image_width + x)*4 + 1];
    int blu = image[(y*image_width + x)*4 + 2];

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
