#include "terrain.h"

// Terrain dimensions:
//
//            height
//            _|_
//        ---|   |-----------
//       /  /     \        /   d
//      /  |       |      /   e
//     /                 /   p
//    /                 /   t
//   /                 /   h
//   ------------------
//        width

// From http://www.exploringbinary.com/ten-ways-to-check-if-an-integer-is-a-power-of-two-in-c/
int isPowerOfTwo (unsigned int x){
    return ((x != 0) && ((x & (~x + 1)) == x));
}

Terrain::Terrain(GLuint shader_program, std::string heightmap_filename, float amplification)
    : Drawable() {
    // This is where generate the new mesh and override the one passed in by
    // the constructor. This is to save space in the game files, so we don't have a terrain mesh

    this->amplification = amplification;

    // First, we load the actual image data, and post to debug
    struct Heightmap heightmap;
    heightmap.image = SOIL_load_image(heightmap_filename.c_str(),
        &(heightmap.width), &(heightmap.height), 0, SOIL_LOAD_RGBA);

    if(heightmap.image){
        Debug::info("Loaded heightmap \"%s\" (%d by %d) into memory.\n",
            heightmap_filename.c_str(), heightmap.width, heightmap.height );
    } else {
        Debug::error("Could not load heightmap \"%s\" into memory.\n",
            heightmap_filename.c_str());
    }

    if(!isPowerOfTwo(heightmap.width) || !isPowerOfTwo(heightmap.height)){
        Debug::warning("Terrain map size is not base 2."
            " Mesh generation may behave incorrectly.\n");
    }

    // After loading in the heightmap to memory, we can make a terrain mesh
    // based on the data
    float start_time = glfwGetTime();
    mesh = generateMesh(heightmap);
    float delta_time = glfwGetTime() - start_time;
    Debug::info("Took %f seconds to generate the terrain mesh.\n", delta_time);

    SOIL_free_image_data(heightmap.image);

    // Once we have a mesh, we can load the drawable data required for this
    // child class.
    Drawable::load(mesh, shader_program, glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 0.0f, 0.0f), 1.0f);

}

int Terrain::getDepth(){
    return scale * depth;
}

int Terrain::getWidth(){
    return scale * width;
}

GLfloat Terrain::getHeight(GLfloat x_pos, GLfloat z_pos){
    // Returns the map height for a specified x and z position.
    // This will be useful for moving units around the terrain.

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
        return 0.0f;
    } else {
        return vertices[i].y;
    }
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

    return normals[i];
}

Mesh* Terrain::generateMesh(Heightmap& heightmap){
    // These two vectors hold the geometry data that will be
    // used to instantiate the Mesh.
    std::vector<GLuint> faces_vector;

    // Starting positions of the mesh
    start_x = -heightmap.width / 2.0;
    start_z = -heightmap.height / 2.0;

    width = heightmap.width;
    depth = heightmap.height;

    // Now, we must generate a mesh from the data in the heightmap.
    float map_height;
    glm::vec3 pos;
    for(int y = 0; y < heightmap.height; ++y){
        for(int x = 0; x < heightmap.width; ++x){
            map_height = getMapHeight(heightmap, x, y);

            // Calculate the vertex position based on the current x, y
            // coordinates, the starting position, and the calculated height
            pos = glm::vec3(start_x + x, map_height, start_z + y);

            // Position Data
            vertices.push_back(pos);
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

    for (int y = 0; y < heightmap.height - 1; ++y){
        for (int x = 0; x < heightmap.width - 1; ++x){
            int y_index = (y * heightmap.width);
            faces_vector.push_back(x + y_index);
            faces_vector.push_back(x + y_index + heightmap.width);
            faces_vector.push_back(x + y_index + 1);

            faces_vector.push_back(x + y_index + 1);
            faces_vector.push_back(x + y_index + heightmap.width);
            faces_vector.push_back(x + y_index + heightmap.width + 1);
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
    normals = std::vector<glm::vec3>(vertices.size());

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

        a = vertices[a_index];
        b = vertices[b_index];
        c = vertices[c_index];

        current_normal = glm::cross((b - a), (c - a));

        normals[a_index] += current_normal;
        normals[b_index] += current_normal;
        normals[c_index] += current_normal;

    }

    // Normalize the normals
    for (int i = 0; i < normals.size(); ++i){
        normals[i] = glm::normalize(normals[i]);
    }

    // Create the map that will be used by level and other things for
    // accessing geometry data from C++.

    // Create the final vertex vector that will be used by OpenGL
    std::vector<GLfloat> texture_repeated_vertices;
    for(int y = 0; y < heightmap.height - 1; ++y){
        for(int x = 0; x < heightmap.width - 1; ++x){

            int i;
            glm::vec3 vertex;
            glm::vec3 normal;
            // Upper left
            i = getIndex(x, y);
            vertex = vertices[i];
            normal = normals[i];

            texture_repeated_vertices.push_back(vertex.x);
            texture_repeated_vertices.push_back(vertex.y);
            texture_repeated_vertices.push_back(vertex.z);

            texture_repeated_vertices.push_back(normal.x);
            texture_repeated_vertices.push_back(normal.y);
            texture_repeated_vertices.push_back(normal.z);

            texture_repeated_vertices.push_back(0.0f);
            texture_repeated_vertices.push_back(0.0f);
            texture_repeated_vertices.push_back(0.0f);

            texture_repeated_vertices.push_back(0.0f);
            texture_repeated_vertices.push_back(0.0f);
            texture_repeated_vertices.push_back(0.0f);

            texture_repeated_vertices.push_back(0.0f);
            texture_repeated_vertices.push_back(0.0f);

            // Upper right
            i = getIndex(x+1, y);
            vertex = vertices[i];
            normal = normals[i];

            texture_repeated_vertices.push_back(vertex.x);
            texture_repeated_vertices.push_back(vertex.y);
            texture_repeated_vertices.push_back(vertex.z);

            texture_repeated_vertices.push_back(normal.x);
            texture_repeated_vertices.push_back(normal.y);
            texture_repeated_vertices.push_back(normal.z);

            texture_repeated_vertices.push_back(0.0f);
            texture_repeated_vertices.push_back(0.0f);
            texture_repeated_vertices.push_back(0.0f);

            texture_repeated_vertices.push_back(0.0f);
            texture_repeated_vertices.push_back(0.0f);
            texture_repeated_vertices.push_back(0.0f);

            texture_repeated_vertices.push_back(1.0f);
            texture_repeated_vertices.push_back(0.0f);

            // Bottom left
            i = getIndex(x, y+1);
            vertex = vertices[i];
            normal = normals[i];

            texture_repeated_vertices.push_back(vertex.x);
            texture_repeated_vertices.push_back(vertex.y);
            texture_repeated_vertices.push_back(vertex.z);

            texture_repeated_vertices.push_back(normal.x);
            texture_repeated_vertices.push_back(normal.y);
            texture_repeated_vertices.push_back(normal.z);

            texture_repeated_vertices.push_back(0.0f);
            texture_repeated_vertices.push_back(0.0f);
            texture_repeated_vertices.push_back(0.0f);

            texture_repeated_vertices.push_back(0.0f);
            texture_repeated_vertices.push_back(0.0f);
            texture_repeated_vertices.push_back(0.0f);

            texture_repeated_vertices.push_back(0.0f);
            texture_repeated_vertices.push_back(1.0f);

            // Bottom right
            i = getIndex(x+1, y+1);
            vertex = vertices[i];
            normal = normals[i];

            texture_repeated_vertices.push_back(vertex.x);
            texture_repeated_vertices.push_back(vertex.y);
            texture_repeated_vertices.push_back(vertex.z);

            texture_repeated_vertices.push_back(normal.x);
            texture_repeated_vertices.push_back(normal.y);
            texture_repeated_vertices.push_back(normal.z);

            texture_repeated_vertices.push_back(0.0f);
            texture_repeated_vertices.push_back(0.0f);
            texture_repeated_vertices.push_back(0.0f);

            texture_repeated_vertices.push_back(0.0f);
            texture_repeated_vertices.push_back(0.0f);
            texture_repeated_vertices.push_back(0.0f);

            texture_repeated_vertices.push_back(1.0f);
            texture_repeated_vertices.push_back(1.0f);

        }
    }

    // Create the final faces vector
    std::vector<GLuint> faces;
    for (int i = 0; i < texture_repeated_vertices.size() / 14.0f; i+= 4){
        faces.push_back(i + 2);
        faces.push_back(i + 1);
        faces.push_back(i + 0);

        faces.push_back(i + 1);
        faces.push_back(i + 2);
        faces.push_back(i + 3);
    }

    Mesh* ground = new Mesh(texture_repeated_vertices, faces);
    return ground;
}

float Terrain::getMapHeight(Heightmap& heightmap, int x, int y){
    // Scaling factor for the height map data
    int red = heightmap.image[(y*heightmap.width + x)*4 + 0];
    int grn = heightmap.image[(y*heightmap.width + x)*4 + 1];
    int blu = heightmap.image[(y*heightmap.width + x)*4 + 2];

    // Scale the height such that the value is between 0.0 and 1.0
    float map_height = float(red + grn + blu)/(3.0f * 255.0);
    // Amplify the map height
    map_height = amplification * map_height;

    return map_height;
}

int Terrain::getIndex(int x, int y){
    // For a given x,y coordinate this will return the
    // index for that element in our linear arrays: vertices,
    // and normals.
    return x + ((width) * y);
}

void Terrain::updateUniformData(){
    // Set the scale, this is not really going to be a thing, probably
    // ^ It's definitely a thing
    glUniform1f(glGetUniformLocation(shader_program, "scale"), scale);

}
