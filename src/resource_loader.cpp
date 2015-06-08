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

    shaders.push_back(Shader(vs_filename, fs_filename));
    return shaders.back();

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
