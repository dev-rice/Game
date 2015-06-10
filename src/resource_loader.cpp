#include "resource_loader.hpp"

ResourceLoader::ResourceLoader() : mesh_path(""), texture_path(""), shader_path("") {

}

Mesh& ResourceLoader::loadMesh(string filename) {
    string mesh_id = mesh_path + filename;
    unordered_map<string, Mesh>::const_iterator key_iter = meshes.find (mesh_id);

    bool has_key = key_iter != meshes.end();

    if (!has_key){
        meshes[mesh_id] = Mesh(mesh_id);
    }
    return meshes[mesh_id];
}

Texture& ResourceLoader::loadTexture(string filename) {
    string texture_id = texture_path + filename;
    unordered_map<string, Texture>::const_iterator key_iter = textures.find (texture_id);

    bool has_key = key_iter != textures.end();

    if (!has_key){
        textures[texture_id] = Texture(texture_id);
    }
    return textures[texture_id];
}

Shader& ResourceLoader::loadShader(string vs_filename, string fs_filename) {
    string shader_id = shader_path + vs_filename + fs_filename;
    unordered_map<string, Shader>::const_iterator key_iter = shaders.find (shader_id);

    bool has_key = key_iter != shaders.end();

    if (!has_key){
        shaders[shader_id] = Shader(vs_filename, fs_filename);
    }
    return shaders[shader_id];

}

void ResourceLoader::setMeshPath(string mesh_path) {
    this->mesh_path = mesh_path;
}

void ResourceLoader::setTexturePath(string texture_path) {
    this->texture_path = texture_path;
}

void ResourceLoader::setShaderPath(string shader_path) {
    this->shader_path = shader_path;
}

string ResourceLoader::getMeshPath() {
        return mesh_path;
}

string ResourceLoader::getTexturePath() {
        return texture_path;
}

string ResourceLoader::getShaderPath() {
        return shader_path;
}
