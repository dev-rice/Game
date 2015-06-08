// Factory class for creating any type of resource and ensuring that there is a place for them to be held in memory

#ifndef ResourceLoader_h
#define ResourceLoader_h

#include <string>

using namespace std;

class ResourceLoader {
public:
    ResourceLoader();

    void setMeshPath(string mesh_path);
    void setTexturePath(string texture_path);
    void setShaderPath(string shader_path);


private:
    string mesh_path;
    string texture_path;
    string shader_path;

};

#endif
