#include "terrain.h"

// From http://www.exploringbinary.com/ten-ways-to-check-if-an-integer-is-a-power-of-two-in-c/
int isPowerOfTwo (unsigned int x){
    return ((x != 0) && ((x & (~x + 1)) == x));
}

Terrain::Terrain(GLuint shader_program, std::string heightmap_filename) : Drawable() {
    // This is where generate the new mesh and override the one passed in by
    // the constructor. This is to save space in the game files, so we don't have a terrain mesh

    // First, we load the actual image data, and post to debug
    image = SOIL_load_image(heightmap_filename.c_str(), &image_width, &image_height, 0, SOIL_LOAD_RGBA);

    if(image){
        Debug::info("Loaded heightmap \"%s\" (%d by %d) into memory.\n", heightmap_filename.c_str(),
            image_width, image_height );
    } else {
        Debug::error("Could not load heightmap \"%s\" into memory.\n", heightmap_filename.c_str());
    }

    if(!isPowerOfTwo(image_width) || !isPowerOfTwo(image_height)){
        Debug::warning("Terrain map size is not base 2. Mesh generation may behave incorrectly.\n");
    }

    // After loading in the heightmap to memory, we can make a terrain mesh
    // based on the data
    float start_time = glfwGetTime();
    mesh = generateMesh();
    float delta_time = glfwGetTime() - start_time;
    Debug::info("Took %f seconds to generate the terrain mesh.\n", delta_time);

    // Once we have a mesh, we can load the drawable data required for this
    // child class.
    Drawable::load(mesh, shader_program, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), 1.0f);

}

Mesh* Terrain::generateMesh(){
    // These two vectors hold the geometry data that will be
    // used to instantiate the Mesh.
    std::vector<glm::vec3> vertices_vector;
    std::vector<GLuint> faces_vector;

    // Starting positions of the mesh
    float start_x = -image_width / 2.0;
    float start_z = -image_height / 2.0;

    // Now, we must generate a mesh from the data in the heightmap. We use
    // 1-BASED INDEXING because of the need for an offset
    float map_height;
    glm::vec3 pos;
    for(int y = 0; y < image_height; ++y){
        for(int x = 0; x < image_width; ++x){
            map_height = getHeight(x, y);

            // Calculate the vertex position based on the current x, y
            // coordinates, the starting position, and the calculated height
            pos = glm::vec3(start_x + x, map_height, start_z + y);

            // Position Data
            vertices_vector.push_back(pos);
        }
    }

    // Generate the face connections in CCW encirclements.
    // For a set of vertices like:
    //      0   1   2
    //      3   4   5
    //      6   7   8
    //
    // Faces (triangles) are:
    //      0   3   1
    //      1   3   4
    //
    //      1   4   2
    //      2   4   5
    //
    //      3   6   4
    //      4   6   7
    //
    //      4   7   5
    //      5   7   8

    for (int y = 0; y < image_height - 1; ++y){
        for (int x = 0; x < image_width - 1; ++x){
            int y_index = (y * image_width);
            faces_vector.push_back(x + y_index);
            faces_vector.push_back(x + y_index + image_width);
            faces_vector.push_back(x + y_index + 1);

            faces_vector.push_back(x + y_index + 1);
            faces_vector.push_back(x + y_index + image_width);
            faces_vector.push_back(x + y_index + image_width + 1);
        }
    }

    // Normal calculations:
    //
    // Calculate a normal for a vertex by taking the cross product of the
    // two edges.
    //
    // For smooth shading, each vertex will have multiple faces attached to it
    // The resultant normal is the sum of the normals for each face (not
    // normalized). This accounts for weighting the normal more for faces with
    // large areas.
    // The normal vector is the unit vector of the sum of each face normal.
    std::vector<glm::vec3> normals(vertices_vector.size());

    GLuint a_index, b_index, c_index;
    glm::vec3 a, b, c;
    glm::vec3 current_normal;
    for (int i = 0; i < faces_vector.size(); i += 3){
        // The normal is uniform across a face so we can
        // calculate the normal for the first vertex in
        // a face and set the others to that.
        a_index = faces_vector[i];
        b_index = faces_vector[i + 1];
        c_index = faces_vector[i + 2];

        a = vertices_vector[a_index];
        b = vertices_vector[b_index];
        c = vertices_vector[c_index];

        current_normal = glm::cross((b - a), (c - a));

        normals[a_index] += current_normal;
        normals[b_index] += current_normal;
        normals[c_index] += current_normal;

    }

    // Normalize the normals
    for (int i = 0; i < normals.size(); ++i){
        normals[i] = glm::normalize(normals[i]);
    }

    // Create the final vertex vector
    std::vector<GLfloat> vertices;
    for(int y = 0; y < image_height - 1; ++y){
        for(int x = 0; x < image_width - 1; ++x){

            int i;
            glm::vec3 vertex;
            glm::vec3 normal;
            // Upper left
            i = x + ((image_width) * y);
            vertex = vertices_vector[i];
            normal = normals[i];

            vertices.push_back(vertex.x);
            vertices.push_back(vertex.y);
            vertices.push_back(vertex.z);

            vertices.push_back(normal.x);
            vertices.push_back(normal.y);
            vertices.push_back(normal.z);

            vertices.push_back(0.0f);
            vertices.push_back(0.0f);

            // Upper right
            i = x + ((image_width) * y) + 1;
            vertex = vertices_vector[i];
            normal = normals[i];

            vertices.push_back(vertex.x);
            vertices.push_back(vertex.y);
            vertices.push_back(vertex.z);

            vertices.push_back(normal.x);
            vertices.push_back(normal.y);
            vertices.push_back(normal.z);

            vertices.push_back(1.0f);
            vertices.push_back(0.0f);

            // Bottom left
            i = x + ((image_width) * (y + 1));
            vertex = vertices_vector[i];
            normal = normals[i];

            vertices.push_back(vertex.x);
            vertices.push_back(vertex.y);
            vertices.push_back(vertex.z);

            vertices.push_back(normal.x);
            vertices.push_back(normal.y);
            vertices.push_back(normal.z);

            vertices.push_back(0.0f);
            vertices.push_back(1.0f);

            // Bottom right
            i = x + ((image_width) * (y + 1)) + 1;
            vertex = vertices_vector[i];
            normal = normals[i];

            vertices.push_back(vertex.x);
            vertices.push_back(vertex.y);
            vertices.push_back(vertex.z);

            vertices.push_back(normal.x);
            vertices.push_back(normal.y);
            vertices.push_back(normal.z);

            vertices.push_back(1.0f);
            vertices.push_back(1.0f);

        }
    }

    // Create the final faces vector
    std::vector<GLuint> faces;
    for (int i = 0; i < vertices.size() / 8.0f; i+= 4){
        faces.push_back(i + 2);
        faces.push_back(i + 1);
        faces.push_back(i + 0);

        faces.push_back(i + 1);
        faces.push_back(i + 2);
        faces.push_back(i + 3);
    }

    Mesh* ground = new Mesh(vertices, faces);
    return ground;
}

float Terrain::getHeight(int x, int y){
    // Scaling factor for the height map data
    float amplification = 10.0f;


    int red = image[(y*image_width + x)*4 + 0];
    int grn = image[(y*image_width + x)*4 + 1];
    int blu = image[(y*image_width + x)*4 + 2];

    // Scale the height such that the value is between 0.0 and 1.0
    float map_height = float(red + grn + blu)/(3.0f * 255.0);
    // Amplify the map height
    map_height = amplification * map_height;

    return map_height;
}

void Terrain::attachTextureSet(TextureSet texture_set){
    // Prepare the loacations for textures to load into and give this drawable some textures. These are specific
    // to Drawable for now but will later be moved down to the child classes such that they can specify different
    // amounts and types of textures to use.

    // When we set this uniform we tell the shader that "diffuse_texture" will be loaded from the 0th texture, and so on.
    // The actual images these numbers point to are specified later in bindTextures().
    glUniform1i(glGetUniformLocation(shader_program, "diffuse_texture"), 0);
    glUniform1i(glGetUniformLocation(shader_program, "specular_texture"), 1);
    glUniform1i(glGetUniformLocation(shader_program, "normal_map"), 2);
    glUniform1i(glGetUniformLocation(shader_program, "emissive_texture"), 3);

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

    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, texture_set->normal);

    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, texture_set->emissive);
}

void Terrain::updateUniformData(){
    // Set the scale, this is not really going to be a thing, probably
    // ^ It's definitely a thing
    glUniform1f(glGetUniformLocation(shader_program, "scale"), scale);

    // Tell the shader the current time
    glUniform1f(glGetUniformLocation(shader_program, "time"), (float)glfwGetTime());

}

int Terrain::getHeight(){
    return this->image_height;
}

int Terrain::getWidth(){
    return this->image_width;
}
