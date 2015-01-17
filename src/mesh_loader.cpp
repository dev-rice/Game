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
    loadMeshFromFile(filename);
}

void MeshLoader::loadMeshFromFile(const char* fileName){
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
    //                    P20 - (V20/V10) * P10
    //            T =  ----------------------------
    //                   U20 - (V20 * U10)/(V10)
    //
    //      5. And the Bitangent by
    //
    //                   P10 - (U10) * T
    //            B =   -----------------
    //                         V10
    //
    //      6. Add these into all vertices on the face

    // Ideally the tangent and bitangent would be already written to
    // the object file. For now, this will suffice.
    // The following is not the most eficient way to implement this
    // because it requires creating a new vertex vector.
    std::vector<GLfloat> vertices;
    for (int i = 0; i < final_tris.size(); i += 3){
        std::vector<glm::vec3> points;
        std::vector<glm::vec2> uvs;
        Debug::info("Face %d: \n", i / 3);
        for (int j = 0; j < 3; ++j){
            int vertex_index = final_tris[i + j] * 8;

            glm::vec3 p = glm::vec3(final_verts[vertex_index],
                final_verts[vertex_index + 1], final_verts[vertex_index + 2]);
            glm::vec2 uv = glm::vec2(final_verts[vertex_index + 6],
                final_verts[vertex_index + 7]);
            points.push_back(p);
            uvs.push_back(uv);
        }

        Debug::info("  points: \n");
        Debug::info("    0: (%.4f, %.4f, %.4f)\n", points[0].x, points[0].y, points[0].z);
        Debug::info("    1: (%.4f, %.4f, %.4f)\n", points[1].x, points[1].y, points[1].z);
        Debug::info("    2: (%.4f, %.4f, %.4f)\n", points[2].x, points[2].y, points[2].z);
        Debug::info("  uvs: \n");
        Debug::info("    0: (%.4f, %.4f)\n", uvs[0].x, uvs[0].y);
        Debug::info("    1: (%.4f, %.4f)\n", uvs[1].x, uvs[1].y);
        Debug::info("    2: (%.4f, %.4f)\n", uvs[2].x, uvs[2].y);

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

        Debug::info("  calculated:\n");
        Debug::info("    u10 = %.4f\n", u10);
        Debug::info("    u20 = %.4f\n", u20);
        Debug::info("    v10 = %.4f\n", v10);
        Debug::info("    v20 = %.4f\n", v20);

        Debug::info("    tangent   = (%.4f, %.4f, %.4f)\n", tangent.x,
            tangent.y, tangent.z);
        Debug::info("    bitangent = (%.4f, %.4f, %.4f)\n", bitangent.x,
            bitangent.y, bitangent.z);
        Debug::info("\n");

    }

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
