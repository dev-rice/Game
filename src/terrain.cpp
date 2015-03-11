#include "terrain.h"

// Terrain dimensions:
//
//            height
//            _|_
//        ---|   |-----------
//       /  /     \        /   h
//      /  |       |      /   t
//     /                 /   p
//    /                 /   e
//   /                 /   d
//   ------------------
//        width

Terrain::Terrain(GLuint shader_program, std::string heightmap_filename, float amplification)
    : Drawable() {

    layered_textures = new LayeredTextures(7);

    // This is where generate the new mesh and override the one passed in by
    // the constructor. This is to save space in the game files, so we don't have a terrain mesh

    max_height = amplification;
    // After loading in the heightmap to memory, we can make a terrain mesh
    // based on the data
    float start_time = GameClock::getInstance()->getCurrentTime();
    mesh = generateMesh(heightmap_filename, amplification);
    float delta_time = GameClock::getInstance()->getCurrentTime() - start_time;
    Debug::info("Took %f seconds to generate the terrain mesh.\n", delta_time);

    // Once we have a mesh, we can load the drawable data required for this
    // child class.
    Drawable::load(mesh, shader_program, glm::vec3(0.0f, 0.0f, 0.0f), 1.0f);

    // Could do bit-packing here but it really doesn't matter
    pathing_array = new bool*[depth];
    for(int i = 0; i < depth; ++i){
        pathing_array[i] = new bool[width];
    }

    // Iterate through the pathing array, filling in all the places where we can't go
    for(int z = 0; z < depth; ++z){
        for(int x = 0; x < width; ++x){
            pathing_array[z][x] = (getSteepness(GLfloat(x) + start_x, GLfloat(z) + start_z) < 0.8f);

            if(pathing_array[z][x]){
                // printf("  ");
            } else {
                // printf("██");
            }
        }
        // printf("\n");
    }

    texture_painter = new TexturePainter(layered_textures->getSplatmap(0));

    // Debugging the allowed areas
    // printPathing();

}

void Terrain::paintSplatmap(glm::vec3 mouse_position){
    int x_offset = mouse_position.x - start_x;
    int y_offset = mouse_position.z - start_z;
    texture_painter->paint(x_offset, y_offset);
    //     glBindTexture(GL_TEXTURE_2D, splatmap_painted);
    //     glTexSubImage2D(GL_TEXTURE_2D, 0, x_offset, y_offset, 4, 4, GL_RED, GL_UNSIGNED_BYTE, brush);
}

bool Terrain::canPath(int x, int z){
    x -= int(start_x);
    z -= int(start_z);

    if(x < 0 || z < 0 || x > width - 1 || z > depth - 1){
        return false;
    }

    return pathing_array[z][x];
}

void Terrain::printPathing(){
    Debug::info("Pathing array:\n");
    for (int x = 0; x < width - 1; ++x){
        for (int z = 0; z < depth - 1; ++ z){
            printf(" %d", pathing_array[z][x]);
        }
        printf("\n");
    }
}

GLfloat Terrain::getHeight(GLfloat x_pos, GLfloat z_pos){
    // Returns the map height for a specified x and z position.
    // This will be useful for moving units around the terrain.

    // Just do an integer conversion to get the vertex index.
    int x = x_pos;
    int z = z_pos;

    // Offset the positions by the starting positions of the mesh
    x = x - start_x;
    z = z - start_z;

    // Calculate the index of the current point in the vertex array.
    int i = getIndex(x, z);

    if (i < 0 || i > vertices.size() - 1){
        return 0.0f;
    } else {
        return vertices[i].position.y;
    }
}

GLfloat Terrain::getHeightInterpolated(GLfloat x_pos, GLfloat z_pos){
    // Interpolates in the current square. Because the positions get floored
    // We can say that the unit is always "at" the top left.

    //        o--------o
    //        |1      2|
    //        |        |
    //        |3      4|
    //        o--------o
    //     x
    //   o-->
    // z |
    //   v

    // Get the heights for each point
    GLfloat height_1 = getHeight(x_pos, z_pos);
    GLfloat height_2 = getHeight(x_pos + 1, z_pos);
    GLfloat height_3 = getHeight(x_pos, z_pos + 1);

    // Get the change in height from point 2 to 1 and from 3 to 1
    GLfloat delta_height_2 = height_2 - height_1;
    GLfloat delta_height_3 = height_3 - height_1;

    // Find the fractional component of the
    // x and z position to know how much to
    // weight each height.
    double intpart;
    GLfloat x_mult = modf(x_pos, &intpart);
    GLfloat z_mult = modf(z_pos, &intpart);

    // Calculate the height by adding the initial height to the
    // weighted combination of the other two points.
    GLfloat interpolated_height = height_1 + (x_mult * delta_height_2) +
        (z_mult * delta_height_3);

    return interpolated_height;
}

glm::vec3 Terrain::getNormal(GLfloat x_pos, GLfloat z_pos){
    // Returns the normal vector at the specified x and y position.
    // This is good for knowing how a unit can move across a segment
    // of terrain. For example, if the normal is too steep, the unit
    // won't be able to move on that segment.
    // Just do an integer conversion to get the vertex index.
    // Later this should be interpolated using the normal.
    int x = x_pos;
    int z = z_pos;

    // Offset the positions by the starting positions of the mesh
    x = x - start_x;
    z = z - start_z;

    // Calculate the index of the current point in the vertex array.
    int i = getIndex(x, z);

    if (i < 0 || i > vertices.size() - 1){
        return glm::vec3(0.0f, 0.0f, 0.0f);
    } else {
        return vertices[i].normal;
    }
}

GLfloat Terrain::getSteepness(GLfloat x_pos, GLfloat z_pos){
    glm::vec3 normal = getNormal(x_pos, z_pos);
    GLfloat cosTheta = glm::dot(glm::vec3(0.0, 1.0, 0.0), normal);

    GLfloat steepness = acos(cosTheta);

    return steepness;
}

bool Terrain::isOnTerrain(GLfloat x_pos, GLfloat z_pos, GLfloat tolerance){
    bool is_on_terrain = (x_pos >= (-getWidth() / 2.0) + tolerance) &&
        (x_pos <= (getWidth() / 2.0) - tolerance) &&
        (z_pos >= (-getDepth() / 2.0) + tolerance) &&
        (z_pos <= (getDepth() / 2.0) - tolerance);
    return is_on_terrain;
}

void Terrain::initializeBaseMesh(Heightmap& heightmap){
    // This generates the mesh that will be used for the
    // top level terrain data, like height and normals.
    // This is just the basic layout of the mesh though
    // because the actual mesh that gets drawn needs to
    // have more accurate normals and texture coordinates
    // for actually drawing things.
    vertices = std::vector<TerrainVertex>(width * depth);
    for (int x = 0; x < width; ++x){
        for (int z = 0; z < depth; ++z){
            TerrainVertex current;
            float height = heightmap.getMapHeight(x, z);
            current.position = glm::vec3(x + start_x, height, z + start_z);

            float u = x / (float)width;
            float v = z / (float)depth;
            current.splatcoord = glm::vec2(u, v);

            int index = getIndex(x, z);
            vertices[index] = current;
        }
    }

    // Make the edge loops to simplify the normal
    // calculations.
    std::vector<GLuint> faces;
    for (int x = 0; x < width - 1; ++x){
        for (int z = 0; z < depth - 1; ++z){
            faces.push_back(getIndex(x, z));
            faces.push_back(getIndex(x + 1, z));
            faces.push_back(getIndex(x, z + 1));

            faces.push_back(getIndex(x + 1, z));
            faces.push_back(getIndex(x + 1, z + 1));
            faces.push_back(getIndex(x, z + 1));
        }
    }

    // Dumb normal calculations without hard edge detection
    // These are sufficient for gameplay terrain data (pathing).
    for (int i = 0; i < faces.size(); i += 3){
        TerrainVertex* a = &vertices[faces[i]];
        TerrainVertex* b = &vertices[faces[i + 1]];
        TerrainVertex* c = &vertices[faces[i + 2]];

        glm::vec3 edge1 = b->position - a->position;
        glm::vec3 edge2 = c->position - a->position;

        glm::vec3 normal   = glm::cross(edge2, edge1);
        glm::vec3 tangent  = edge1;
        glm::vec3 binormal = edge2;

        a->normal += normal;
        b->normal += normal;
        c->normal += normal;

        a->tangent += tangent;
        b->tangent += tangent;
        c->tangent += tangent;

        a->binormal += binormal;
        b->binormal += binormal;
        c->binormal += binormal;

    }

    // Normalize the normals
    for (int i = 0; i < vertices.size(); ++i){
        TerrainVertex* current = &vertices[i];
        current->normal = glm::normalize(current->normal);
        current->tangent = glm::normalize(current->tangent);
        current->binormal = glm::normalize(current->binormal);

    }
}

Mesh* Terrain::generateMesh(std::string filename, float amplification){
    Heightmap heightmap = Heightmap(filename, amplification);

    width = heightmap.getWidth();
    depth = heightmap.getHeight();

    start_x = -width / 2;
    start_z = -depth / 2;

    // Generate the mesh for gameplay data
    initializeBaseMesh(heightmap);

    // Now make it look nice!

    // The number of terrain tiles before the
    // texture repeats.
    int texture_size = 16;

    // This should probably definitely be rewritten because
    // it will be hard to do the normal fix afterwards. Also
    // its so many loops!
    std::vector<GLuint> faces;
    std::vector<TerrainVertex> textured_vertices;
    std::unordered_map<int, bool> valid_vertices;
    for (int x = 0; x < width; x += texture_size){
        for (int z = 0; z < depth; z += texture_size){
            int start_index = textured_vertices.size();

            // Create the big tile
            for (int i = 0; i <= texture_size; ++i){
                for (int j = 0; j <= texture_size; ++j){
                    int index = getIndex(x + i, z + j);
                    float u = i / (float)(texture_size);
                    float v = j / (float)(texture_size);

                    if (index < vertices.size()){
                        TerrainVertex current = vertices[index];
                        current.texcoord = glm::vec2(u, v);
                        textured_vertices.push_back(current);
                    } else {
                        TerrainVertex current;
                        textured_vertices.push_back(current);
                    }

                    int textured_index = textured_vertices.size() - 1;
                    if (((x + i) >= width) || ((z + j) >= depth)){
                        valid_vertices[textured_index] = false;
                    } else {
                        valid_vertices[textured_index] = true;
                    }
                }
            }

            for (int i = 0; i < texture_size; ++i){
                for (int j = 0; j < texture_size; ++j){
                    int block_width = texture_size + 1;

                    int upper_left  = start_index + getIndex(i, j, block_width);
                    int upper_right = start_index + getIndex(i + 1, j, block_width);
                    int lower_left  = start_index + getIndex(i, j + 1, block_width);
                    int lower_right = start_index + getIndex(i + 1, j + 1, block_width);

                    bool is_valid = true;
                    is_valid &= valid_vertices[upper_left];
                    is_valid &= valid_vertices[upper_right];
                    is_valid &= valid_vertices[lower_left];
                    is_valid &= valid_vertices[lower_right];

                    if (is_valid){
                        faces.push_back(upper_left);
                        faces.push_back(upper_right);
                        faces.push_back(lower_left);

                        faces.push_back(upper_right);
                        faces.push_back(lower_right);
                        faces.push_back(lower_left);
                    }

                }
            }

        }
    }

    return new TerrainMesh(textured_vertices, faces);
}

int Terrain::getIndex(int x, int z){
    // For a given x,z coordinate this will return the
    // index for that element in our linear arrays: vertices,
    // and normals.
    return x + ((width) * z);
}

int Terrain::getIndex(int x, int z, int width){
    // For a given x,z coordinate this will return the
    // index for that element in our linear arrays: vertices,
    // and normals.
    return x + ((width) * z);
}

void Terrain::updateUniformData(){
    // Set the scale, this is not really going to be a thing, probably
    // ^ It's definitely a thing
    GLuint scale_loc = glGetUniformLocation(shader_program, "scale");
    GLuint time_loc = glGetUniformLocation(shader_program, "time");
    //
    // std::string shader_name = ShaderLoader::getShaderName(shader_program);
    // Debug::info("Shader Program: %s\n", shader_name.c_str());
    // Debug::info("  scale_loc = %d\n", scale_loc);
    // Debug::info("  time_loc = %d\n", time_loc);

    glUniform1f(scale_loc, scale);
    glUniform1f(time_loc, GameClock::getInstance()->getCurrentTime());

}

void Terrain::bindTextures(){
    // Put each texture into the correct location for this Drawable. GL_TEXTURE0-3
    // correspond to the uniforms set in attachTextureSet(). This is where we actually
    // tell the graphics card which textures to use.

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, specular);

    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, emissive);

    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, normal);

    layered_textures->updateUniforms(shader_program);

}

void Terrain::setTextureLocations(){
    // Try to set the texture locations
    glUniform1i(glGetUniformLocation(shader_program, "specular_texture"), 1);
    glUniform1i(glGetUniformLocation(shader_program, "emissive_texture"), 2);
    glUniform1i(glGetUniformLocation(shader_program, "normal_map"), 3);
    glUniform1i(glGetUniformLocation(shader_program, "shadow_map"), 4);

    layered_textures->setTextureLocations(shader_program);

}

GLuint getChannelIndex(char channel){
    if (channel == 'r'){
        return 1;
    } else if (channel == 'g'){
        return 2;
    } else if (channel == 'b'){
        return 3;
    } else {
        return 0;
    }
}

void Terrain::addSplatmap(GLuint splat){
    texture_painter->setTexture(splat);
    layered_textures->addSplatmap(splat);
}

void Terrain::addDiffuse(GLuint diff, GLuint splat, int layer_num, char channel) {
    layered_textures->addTexture(diff, splat, channel, layer_num);

    Drawable::setDiffuse(diff);
}
