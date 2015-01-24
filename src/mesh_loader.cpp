// mesh_loader.cpp
// implementation of the model class
// Trevor Westphal

#include <vector>  // vector
#include <fstream> // fgets
#include <stdio.h> // sscanf
#include <map>     // map
#include <array>   // array

#include "mesh_loader.h"

MeshLoader::MeshLoader(const char* filename){
    loadMeshFromOBJ(filename);
}

void MeshLoader::loadMeshFromOBJ(const char* filename){
    float start_time = glfwGetTime();

    float tempX, tempY, tempZ;
    int tempA, tempB, tempC, tempD, tempE, tempF, tempG, tempH, tempI;

    std::vector<GLfloat> verts3D;
    std::vector<GLfloat> verts2D;
    std::vector<GLfloat> verts_norm;
    std::vector<GLuint> tris3D;
    std::vector<GLuint> tris2D;
    std::vector<GLuint> norms3D;

    std::vector<glm::vec3> tangents;
    std::vector<glm::vec3> bitangents;


    int triCount = 0;

    char buffer[128];

    FILE *ifile;
    ifile = fopen(filename, "r");

    if(ifile == NULL){
        Debug::error("Error opening file %s\n", filename);
        return;

    }

    while(! feof(ifile)){
        if(fgets(buffer, 128, ifile) == NULL){
            // Can't read into buffer
            break;
        }

        if(buffer[0] == 'v'){
            if(buffer[1] == 't'){
                // texture vertex
                sscanf(buffer, "%*2c %f %f", &tempX, &tempY);
                verts2D.push_back((GLfloat)tempX);
                verts2D.push_back((GLfloat)(1.0f - tempY));
            } else if (buffer[1] == 'n'){
                sscanf(buffer, "%*2c %f %f %f", &tempX, &tempY, &tempZ);
                verts_norm.push_back((GLfloat)tempX);
                verts_norm.push_back((GLfloat)tempY);
                verts_norm.push_back((GLfloat)tempZ);
            } else {
                // 3d vertex
                sscanf(buffer, "%*c %f %f %f", &tempX, &tempY, &tempZ);
                verts3D.push_back((GLfloat)tempX);
                verts3D.push_back((GLfloat)tempY);
                verts3D.push_back((GLfloat)tempZ);
            }
        } else if(buffer[0] == 'f'){
            // Face declarations
            sscanf(buffer, "%*c %d/%d/%d %d/%d/%d %d/%d/%d", &tempA, &tempD, &tempG, &tempB, &tempE, &tempH, &tempC, &tempF, &tempI);
            tris3D.push_back((GLuint)tempA);
            tris3D.push_back((GLuint)tempB);
            tris3D.push_back((GLuint)tempC);
            tris2D.push_back((GLuint)tempD);
            tris2D.push_back((GLuint)tempE);
            tris2D.push_back((GLuint)tempF);
            norms3D.push_back((GLuint)tempG);
            norms3D.push_back((GLuint)tempH);
            norms3D.push_back((GLuint)tempI);
            triCount++;
        }
    }

    fclose(ifile);

    // Setting up the data structure to see if vertex mapping has already been done
    int count = 1;

    std::map<std::array<GLuint, 2>, GLuint> mappedEdgeLoops;

    for(int i(0); i < triCount; ++i){

        for(int j(0); j < 3; ++j){

            std::array<GLuint, 2> tuple = {tris3D[i*3+j], tris2D[i*3+j]};

            int result = mappedEdgeLoops[tuple];

            if(result == 0){
                // Has not been declared yet
                mappedEdgeLoops[tuple] = count;
                count++;

                // Acessing 3D vertices indicated by the connection list
                tempX = verts3D[ (tuple[0]-1)*3+0 ];
                tempY = verts3D[ (tuple[0]-1)*3+1 ];
                tempZ = verts3D[ (tuple[0]-1)*3+2 ];

                // Acessing 3D normal vertices indicated by the connection list
                GLuint something = norms3D[i*3 + j];
                GLfloat tempVN1 = verts_norm[(something - 1)*3 +0 ];
                GLfloat tempVN2 = verts_norm[ (something - 1)*3 +1 ];
                GLfloat tempVN3 = verts_norm[(something - 1)*3 +2  ];

                // Acessing 2D vertices indicated by the connection list
                GLfloat tempX2 = verts2D[ (tuple[1]-1)*2+0 ];
                GLfloat tempY2 = verts2D[ (tuple[1]-1)*2+1 ];

                final_verts.push_back(tempX);
                final_verts.push_back(tempY);
                final_verts.push_back(tempZ);
                final_verts.push_back(tempVN1);
                final_verts.push_back(tempVN2);
                final_verts.push_back(tempVN3);
                final_verts.push_back(tempX2);
                final_verts.push_back(tempY2);
            }
            final_tris.push_back(mappedEdgeLoops[tuple]-1);
        }
    }

    // Define the basis of the space that each vertex is at for
    // normal mapping.
    // For each face:
    //      1. The first point is P0, second is P1, third is P2
    //      2. Find P10 and P20 which are vectors defining the edges
    //      3. Find U10, U20, V10, and V20 which are scalars that
    //         represent the change in texture coordinates for each
    //         point.
    //      4. Calculate the Tangent by
    //
    //                    P10 * V20 - P20 * V10
    //            T =  ----------------------------
    //                    U10 * V20 - U20 * V10
    //
    //      5. And the Bitangent by
    //
    //                    P20 * U10 - P10 * U20
    //            T =  ----------------------------
    //                    U10 * V20 - U20 * V10
    //
    //      6. Add these into all vertices on the face

    // Ideally the tangent and bitangent would be already written to
    // the object file. For now, this will suffice.
    // The following is not the most eficient way to implement this
    // because it requires creating a new vertex vector and a bunch of
    // other stuff that was done already.
    int num_vertices = final_verts.size() / 8;
    std::vector<GLfloat> vertices = std::vector<GLfloat>(num_vertices * 14);

    for (int i = 0; i < final_tris.size(); i += 3){
        std::vector<glm::vec3> points;
        std::vector<glm::vec3> normals;
        std::vector<glm::vec2> uvs;

        for (int j = 0; j < 3; ++j){
            int index = final_tris[i + j] * 8;

            glm::vec3 p = glm::vec3(final_verts[index],
                final_verts[index + 1], final_verts[index + 2]);
            glm::vec3 n = glm::vec3(final_verts[index + 3],
                final_verts[index + 4], final_verts[index + 5]);
            glm::vec2 uv = glm::vec2(final_verts[index + 6],
                final_verts[index + 7]);

            points.push_back(p);
            normals.push_back(n);
            uvs.push_back(uv);
        }

        glm::vec3 p10 = points[1] - points[0];
        glm::vec3 p20 = points[2] - points[0];

        float u10 = uvs[1].x - uvs[0].x;
        float u20 = uvs[2].x - uvs[0].x;
        float v10 = uvs[1].y - uvs[0].y;
        float v20 = uvs[2].y - uvs[0].y;

        // Need to check for edge cases because this math
        // breaks if ((u10 * v20) - (v10 * u20)) = 0
        float divisor = 1.0f / (u10 * v20 - v10 * u20);
        glm::vec3 tangent = (p10 * v20 - p20 * v10) * divisor;
        glm::vec3 bitangent = (p20 * u10 - p10 * u20) * divisor;

        tangent = glm::normalize(tangent);
        bitangent = glm::normalize(bitangent);

        // Actually set this data in the vertex array
        for (int j = 0; j < 3; ++j){
            int index = final_tris[i + j] * 14;

            glm::vec3 point = points[j];
            glm::vec3 normal = normals[j];
            glm::vec2 uv = uvs[j];

            normal = glm::normalize(normal);

            // Shitty Shitty Shit Shit code
            vertices[index]      = point.x;
            vertices[index + 1]  = point.y;
            vertices[index + 2]  = point.z;
            vertices[index + 3]  = normal.x;
            vertices[index + 4]  = normal.y;
            vertices[index + 5]  = normal.z;
            vertices[index + 6]  = tangent.x;
            vertices[index + 7]  = tangent.y;
            vertices[index + 8]  = tangent.z;
            vertices[index + 9]  = bitangent.x;
            vertices[index + 10] = bitangent.y;
            vertices[index + 11] = bitangent.z;
            vertices[index + 12] = uv.x;
            vertices[index + 13] = uv.y;
        }
    }

    final_verts = vertices;

    float delta_time = glfwGetTime() - start_time;
    Debug::info("Obj mesh loaded from %s in %.5f seconds.\n", filename, delta_time);

}

std::vector<float> getFloatsFromString(std::string input, char delim){
    std::vector<float> result;
    std::string tmp;
    std::string::iterator i;
    result.clear();

    for(i = input.begin(); i <= input.end(); ++i) {
        if((const char)*i != delim  && i != input.end()) {
            tmp += *i;
        } else {
            result.push_back(std::stof(tmp));
            tmp = "";
        }
    }

    return result;
}

std::vector<int> getIntsFromString(std::string input, char delim){
    std::vector<int> result;
    std::string tmp;
    std::string::iterator i;
    result.clear();

    for(i = input.begin(); i <= input.end(); ++i) {
        if((const char)*i != delim  && i != input.end()) {
            tmp += *i;
        } else {
            result.push_back(std::stoi(tmp));
            tmp = "";
        }
    }

    return result;
}

std::vector<glm::vec3> getVec3FromString(std::string input){
    std::vector<glm::vec3> result;
    std::string tmp;
    std::string::iterator i;
    char delim = ' ';

    glm::vec3 current_vector;
    int count = 0;
    for(i = input.begin(); i <= input.end(); ++i) {
        if((const char)*i != delim  && i != input.end()) {
            tmp += *i;
        } else {
            count++;
            float value = std::stof(tmp);
            if (count == 1){
                current_vector.x = value;
            } else if (count == 2){
                current_vector.y = value;
            } else if (count == 3){
                current_vector.z = value;
                result.push_back(current_vector);
                current_vector = glm::vec3();
                count = 0;
            }
            tmp = "";
        }
    }

    return result;
}

std::vector<glm::vec2> getVec2FromString(std::string input){
    std::vector<glm::vec2> result;
    std::string tmp;
    std::string::iterator i;
    char delim = ' ';

    glm::vec2 current_vector;
    int count = 0;
    for(i = input.begin(); i <= input.end(); ++i) {
        if((const char)*i != delim  && i != input.end()) {
            tmp += *i;
        } else {
            count++;
            float value = std::stof(tmp);
            if (count == 1){
                current_vector.x = value;
            } else if (count == 2){
                current_vector.y = value;
                result.push_back(current_vector);
                current_vector = glm::vec2();
                count = 0;
            }
            tmp = "";
        }
    }

    return result;
}

void MeshLoader::loadMeshFromDAE(const char* filename){
    float start_time = glfwGetTime();

    // Load the document into a pugixml object
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(filename);

    // Get the node that contains geometry data for each mesh.
    pugi::xml_node mesh_list_node = doc.child("COLLADA").child("library_geometries");

    // For now we don't really support multiple mesh loading from one file
    // but this is still good to have
    for (pugi::xml_node geometry_node : mesh_list_node.children()){
        std::vector<glm::vec3> vertices;
        std::vector<glm::vec3> normals;
        std::vector<glm::vec2> texcoords;
        std::vector<int> faces;

        std::string mesh_id = std::string(geometry_node.attribute("id").as_string());

        std::string mesh_vertex_source_id = mesh_id + "-positions";
        std::string mesh_normal_source_id = mesh_id + "-normals";
        std::string mesh_uv_source_id = mesh_id + "-map-0";

        // The first child in a geometry node contains all the data
        // relevant to the mesh.
        pugi::xml_node mesh_node = geometry_node.first_child();

        for (pugi::xml_node mesh_data_node : mesh_node.children()){
            // If it is a source node
            if (strcmp(mesh_data_node.name(), "source") == 0){
                std::string source_id = std::string(mesh_data_node.attribute("id").as_string());
                // Get the count and stride for error checking.
                pugi::xml_node accessor = mesh_data_node.child("technique_common").child("accessor");
                int stride = accessor.attribute("stride").as_int();

                if (source_id == mesh_vertex_source_id){
                    // Check if the array width is correct for the vertices
                    if (stride != 3){
                        Debug::error("Invalid array width for vertex array in %s",
                            filename);
                    } else {
                        std::string vertex_array_string = mesh_data_node.child_value("float_array");
                        vertices = getVec3FromString(vertex_array_string);
                    }

                } else if (source_id == mesh_normal_source_id){
                    // Check if the array width is correct for the normals
                    if (stride != 3){
                        Debug::error("Invalid array width for normal array in %s",
                            filename);
                    } else {
                        std::string normal_array_string = mesh_data_node.child_value("float_array");
                        normals = getVec3FromString(normal_array_string);
                    }

                } else if (source_id == mesh_uv_source_id){
                    // Check if the array width is correct for the texture
                    // coordinates
                    if (stride != 2){
                        Debug::error("Invalid array width for uv array in %s",
                            filename);
                    } else {
                        std::string uv_array_string = mesh_data_node.child_value("float_array");
                        texcoords = getVec2FromString(uv_array_string);
                    }

                }

            } else if (strcmp(mesh_data_node.name(), "polylist") == 0){
                // Check that the vertex counts are all three (triangles)
                std::string vcount_str = mesh_data_node.child_value("vcount");

                // Get the list of faces
                std::string faces_str = mesh_data_node.child_value("p");
                faces = getIntsFromString(faces_str, ' ');
            }
        }

        if (!vertices.empty() && !normals.empty() && !texcoords.empty()){
            Debug::info("Mesh data loaded successfully.\n");

            Debug::info("Vertices:\n");
            for (glm::vec3 vertex : vertices){
                Debug::info("  %f, %f, %f\n", vertex.x, vertex.y, vertex.z);
            }

            Debug::info("Normals:\n");
            for (glm::vec3 normal : normals){
                Debug::info("  %f, %f, %f\n", normal.x, normal.y, normal.z);
            }

            Debug::info("Texcoords:\n");
            for (glm::vec2 uv : texcoords){
                Debug::info("  %f, %f\n", uv.x, uv.y);
            }
        } else {
            Debug::error("Error loading mesh data from '%s'.\n", filename);
        }

    }

    float delta_time = glfwGetTime() - start_time;
    Debug::info("Collada mesh loaded from %s in %.5f seconds.\n", filename, delta_time);
}

std::vector<GLfloat> MeshLoader::getVertexArray(){
    // GLfloat* vertices = new GLfloat[final_verts.size()];
    // for (int i = 0; i < final_verts.size(); ++i){
    //     vertices[i] = final_verts[i];
    // }
    return final_verts;
}

std::vector<GLuint> MeshLoader::getFaceArray(){
    // GLuint* faces = new GLuint[final_tris.size()];
    // for (int i = 0; i < final_tris.size(); ++i){
    //     faces[i] = final_tris[i];
    // }
    return final_tris;
}
