// model.cpp
// implementation of the model class
// Trevor Westphal

#include <vector>  // vector
#include <fstream> // fgets
#include <stdio.h> // sscanf
#include <map>	   // map
#include <array>   // array

#include "mesh.h"

Mesh::Mesh(const char* filename){
	loadMeshFromFile(filename);
}

void Mesh::loadMeshFromFile(const char* fileName){
	float tempX, tempY, tempZ;
	int tempA, tempB, tempC, tempD, tempE, tempF;

	std::vector<GLfloat> verts3D;
	std::vector<GLfloat> verts2D;
	std::vector<GLuint> tris3D;
	std::vector<GLuint> tris2D;

	int triCount = 0;

	char buffer[128];

	FILE *ifile;
	ifile = fopen(fileName, "r");

	if(ifile == NULL){
		printf("Error opening file %s", fileName);
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
			} else {
				// 3d vertex
				sscanf(buffer, "%*c %f %f %f", &tempX, &tempY, &tempZ);
				verts3D.push_back((GLfloat)tempX);
				verts3D.push_back((GLfloat)tempY);
				verts3D.push_back((GLfloat)tempZ);
			}
		} else if(buffer[0] == 'f'){
			// Face declarations
			sscanf(buffer, "%*c %d/%d %d/%d %d/%d", &tempA, &tempD, &tempB, &tempE, &tempC, &tempF);
			tris3D.push_back((GLuint)tempA);
			tris3D.push_back((GLuint)tempB);
			tris3D.push_back((GLuint)tempC);
			tris2D.push_back((GLuint)tempD);
			tris2D.push_back((GLuint)tempE);
			tris2D.push_back((GLuint)tempF);
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
				// Acessing 2D vertices indicated by the connection list
				GLfloat tempX2 = verts2D[ (tuple[1]-1)*2+0 ];
				GLfloat tempY2 = verts2D[ (tuple[1]-1)*2+1 ];

				final_verts.push_back(tempX);
				final_verts.push_back(tempY);
				final_verts.push_back(tempZ);
				final_verts.push_back(1.0f);
				final_verts.push_back(1.0f);
				final_verts.push_back(1.0f);
				final_verts.push_back(tempX2);
				final_verts.push_back(tempY2);
			}	
			final_tris.push_back(mappedEdgeLoops[tuple]-1);		
		}
	}
}

GLfloat* Mesh::getVertexArray(){
	GLfloat* vertices = new GLfloat[final_verts.size()];
	for (int i = 0; i < final_verts.size(); ++i){
		vertices[i] = final_verts[i];
	}
	return vertices;
}

GLuint* Mesh::getFaceArray(){
	GLuint* faces = new GLuint[final_tris.size()];
	for (int i = 0; i < final_tris.size(); ++i){
		faces[i] = final_tris[i];
	}
	return faces;
}

int Mesh::getVerticesSize(){
	return final_verts.size();
}

int Mesh::getFacesSize(){
	return final_tris.size();
}
