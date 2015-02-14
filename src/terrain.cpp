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
    // This is where generate the new mesh and override the one passed in by
    // the constructor. This is to save space in the game files, so we don't have a terrain mesh

    // After loading in the heightmap to memory, we can make a terrain mesh
    // based on the data
    float start_time = glfwGetTime();
    mesh = generateMesh(heightmap_filename, amplification);
    float delta_time = glfwGetTime() - start_time;
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
        }
    }

    // Debugging the allowed areas
    // printPathing();
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

Mesh* Terrain::generateMesh(std::string filename, float amplification){
    Heightmap heightmap = Heightmap(filename, amplification);

    width = heightmap.getWidth();
    depth = heightmap.getHeight();

    start_x = -width / 2;
    start_z = -depth / 2;

    vertices = std::vector<Vertex>(width * depth);
    std::vector<GLuint> faces;

    for (int x = 0; x < width; ++x){
        for (int z = 0; z < depth; ++z){
            Vertex current;
            float height = heightmap.getMapHeight(x, z);
            current.position = glm::vec3(x + start_x, height, z + start_z);
            current.normal   = glm::vec3(0.0f, 1.0f, 0.0f);
            current.tangent  = glm::vec3(1.0f, 0.0f, 0.0f);
            current.binormal = glm::vec3(0.0f, 0.0f, 1.0f);
            current.texcoord = glm::vec2((float)x / (float)width,
                (float)z / (float)depth);

            int index = getIndex(x, z);
            vertices[index] = current;
        }
    }

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

    std::vector<GLfloat> mesh_vertices;
    for (int x = 0; x < width; ++x){
        for (int z = 0; z < depth; ++z){
            int index = getIndex(x, z);
            Vertex current = vertices[index];
            mesh_vertices.push_back(current.position.x);
            mesh_vertices.push_back(current.position.y);
            mesh_vertices.push_back(current.position.z);
            mesh_vertices.push_back(current.normal.x);
            mesh_vertices.push_back(current.normal.y);
            mesh_vertices.push_back(current.normal.z);
            mesh_vertices.push_back(current.tangent.x);
            mesh_vertices.push_back(current.tangent.y);
            mesh_vertices.push_back(current.tangent.z);
            mesh_vertices.push_back(current.binormal.x);
            mesh_vertices.push_back(current.binormal.y);
            mesh_vertices.push_back(current.binormal.z);
            mesh_vertices.push_back(current.texcoord.x);
            mesh_vertices.push_back(current.texcoord.y);
        }
    }
    return new Mesh(mesh_vertices, faces);
}

int Terrain::getIndex(int x, int z){
    // For a given x,z coordinate this will return the
    // index for that element in our linear arrays: vertices,
    // and normals.
    return x + ((width) * z);
}

void Terrain::updateUniformData(){
    // Set the scale, this is not really going to be a thing, probably
    // ^ It's definitely a thing
    glUniform1f(glGetUniformLocation(shader_program, "scale"), scale);

}
