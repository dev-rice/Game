#include "resource_loader.hpp"

ResourceLoader::ResourceLoader() {

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
