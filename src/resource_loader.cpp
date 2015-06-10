#include "resource_loader.hpp"

ResourceLoader::ResourceLoader() {

}

Mesh& ResourceLoader::loadMesh(string filename) {
    meshes.push_back(Mesh(filename));
    return meshes.back();
}

Texture& ResourceLoader::loadTexture(string filename) {
    textures.push_back(Texture(filename));
    return textures.back();
}

Shader& ResourceLoader::loadShader(string vs_filename, string fs_filename) {
    string shader_id = vs_filename + fs_filename;
    unordered_map<string, Shader>::const_iterator key_iter = shaders.find (shader_id);

    bool has_key = key_iter != shaders.end();

    if (!has_key){
        shaders[shader_id] = Shader(vs_filename, fs_filename);
    }
    return shaders[shader_id];

}

void ResourceLoader::addMeshPath(string mesh_path) {
    mesh_paths.push_back(mesh_path);
}

void ResourceLoader::addTexturePath(string texture_path) {
    texture_paths.push_back(texture_path);
}

void ResourceLoader::addShaderPath(string shader_path) {
    shader_paths.push_back(shader_path);
}

string ResourceLoader::getDefaultMeshPath() {
    if (mesh_paths.empty()){
        return "";
    } else {
        return mesh_paths[0];
    }
}

string ResourceLoader::getDefaultTexturePath() {
    if (mesh_paths.empty()){
        return "";
    } else {
        return texture_paths[0];
    }
}

string ResourceLoader::getDefaultShaderPath() {
    if (mesh_paths.empty()){
        return "";
    } else {
        return shader_paths[0];
    }
}
