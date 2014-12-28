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

    // After loading in the heightmap to memory, we can make a terrain mesh
    // based on the data
    mesh = generateMesh();

    // Once we have a mesh, we can load the drawable data required for this
    // child class.
    Drawable::load(mesh, shader_program, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), 1.0f);

}

Mesh* Terrain::generateMesh(){
    // These two vectors hold the geometry data that will be
    // used to instantiate the Mesh.
    std::vector<GLfloat> vertices_vector;
    std::vector<GLuint> faces_vector;

    // Starting positions of the mesh
    float start_x = -image_width / 2.0;
    float start_z = -image_height / 2.0;

    // These will map the x, y position on the image
    // to u, v coordinates for the texture.
    // TODO: Make this tile instead of load the entire
    // image over the mesh.
    float u_inc = 1.0 / (float)image_width;
    float v_inc = 1.0 / (float)image_height;

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
            // printf("%f\t", map_height);

            // Position Data
            vertices_vector.push_back(pos.x);
            vertices_vector.push_back(pos.y);
            vertices_vector.push_back(pos.z);

            // Normal calculations need to be done
            vertices_vector.push_back(0.0f);
            vertices_vector.push_back(1.0f);
            vertices_vector.push_back(0.0f);

            // Texture Coordinate Data
            vertices_vector.push_back(u_inc * x);
            vertices_vector.push_back(v_inc * y);

        }
        // printf("\n");
    }

    // Generate the face connections in CCW encirclements.
    // For a set of vertices like:
    //      0   1   2
    //      3   4   5
    //      6   7   8
    //      9   10  11
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
    //
    //      6   9   7
    //      7   9   10
    //
    //      7   10  8
    //      8   10  11


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
    int number_of_normals = vertices_vector.size() / 8.0f;
    std::vector<glm::vec3> normals(number_of_normals);

    GLuint A_index, B_index, C_index;
    glm::vec3 A, B, C;
    glm::vec3 current_normal;
    for (int i = 0; i < faces_vector.size(); i += 3){
        // The normal is uniform across a face so we can
        // calculate the normal for the first vertex in
        // a face and set the others to that.
        A_index = faces_vector[i];
        B_index = faces_vector[i + 1];
        C_index = faces_vector[i + 2];

        A = getVertexPosition(A_index, &vertices_vector);
        B = getVertexPosition(B_index, &vertices_vector);
        C = getVertexPosition(C_index, &vertices_vector);

        current_normal = glm::cross((B - A), (C - A));

        normals[A_index] += current_normal;
        normals[B_index] += current_normal;
        normals[C_index] += current_normal;

    }

    for (int i = 0; i < normals.size(); ++i){
        normals[i] = glm::normalize(normals[i]);
        glm::vec3 normal = normals[i];
        // Debug::info("%d: <%f, %f, %f>\n", i, normal.x, normal.y, normal.z);

        int index = i * 8;
        vertices_vector[index + 3] = normal.x;
        vertices_vector[index + 4] = normal.y;
        vertices_vector[index + 5] = normal.z;

    }

    Mesh* ground = new Mesh(vertices_vector, faces_vector);
    return ground;
}

glm::vec3 Terrain::getVertexPosition(GLuint index, std::vector<GLfloat>* vertices){
    glm::vec3 pos;
    index = index * 8;

    pos.x = vertices->at(index);
    pos.y = vertices->at(index + 1);
    pos.z = vertices->at(index + 2);

    return pos;
}

float Terrain::getHeight(int x, int y){
    // Scaling factor for the height map data
    float amplification = 20.0f;


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
