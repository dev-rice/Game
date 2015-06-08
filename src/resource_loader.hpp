// Factory class for creating any type of resource and ensuring that there is a place for them to be held in memory

#ifndef ResourceLoader_h
#define ResourceLoader_h

#include <string>
#include <vector>

#include "mesh.h"
#include "texture.hpp"
#include "shader.hpp"

using namespace std;

class ResourceLoader {
public:
    ResourceLoader();

    Mesh& loadMesh(string filename);
    Texture& loadTexture(string filename);
    Shader& loadShader(string vs_filename, string fs_filename);

    void addMeshPath(string mesh_path);
    void addTexturePath(string texture_path);
    void addShaderPath(string shader_path);

private:
    vector<Mesh> meshes;
    vector<Texture> textures;
    vector<Shader> shaders;

    vector<string> mesh_paths;
    vector<string> texture_paths;
    vector<string> shader_paths;

};

#endif
