#include "doodad.h"

Doodad::Doodad(const Json::Value& doodad_json, std::string mesh_path, std::string texture_path){

    std::string mesh_filename = doodad_json["mesh"].asString();
    std::string full_path = mesh_path + mesh_filename;
    // NOOOOOOOO!!!!!! but for now...
    Mesh* mesh_ptr = new Mesh(full_path);

    // Scale of the doodad
    float scale = doodad_json["scale"].asFloat();

    // World position of the doodad
    glm::vec3 position;
    position.x = doodad_json["position"]["x"].asFloat();
    position.y = doodad_json["position"]["y"].asFloat();
    position.z = doodad_json["position"]["z"].asFloat();
    // Euler rotation of the doodad
    glm::vec3 rotation;
    rotation.x = doodad_json["rotation"]["x"].asFloat();
    rotation.y = doodad_json["rotation"]["y"].asFloat();
    rotation.z = doodad_json["rotation"]["z"].asFloat();

    GLuint shader_program = ShaderLoader::loadShaderProgram("shaders/doodad.vs", "shaders/doodad.fs");
    load(mesh_ptr, shader_program, position, scale);
    setRotationEuler(rotation);

    // Load the textures
    const Json::Value textures = doodad_json["textures"];
    std::string diffuse_filename = textures["diff"].asString();
    std::string normal_filename = textures["norm"].asString();
    std::string specular_filename = textures["spec"].asString();
    std::string emissive_filename = textures["emit"].asString();

    if (diffuse_filename != ""){
        diffuse_filename = texture_path + diffuse_filename;
        GLuint diff = TextureLoader::loadTextureFromFile(diffuse_filename, GL_LINEAR);
        setDiffuse(diff);
    }
    if (normal_filename != ""){
        normal_filename = texture_path + normal_filename;
        GLuint norm = TextureLoader::loadTextureFromFile(normal_filename, GL_LINEAR);
        setNormal(norm);
    }
    if (specular_filename != ""){
        specular_filename = texture_path + specular_filename;
        GLuint spec = TextureLoader::loadTextureFromFile(specular_filename, GL_LINEAR);
        setSpecular(spec);
    }
    if (emissive_filename != ""){
        emissive_filename = texture_path + emissive_filename;
        GLuint emit = TextureLoader::loadTextureFromFile(emissive_filename, GL_LINEAR);
        setEmissive(emit);
    }

}

Doodad::Doodad(Mesh* mesh) {
    GLuint shader_program = ShaderLoader::loadShaderProgram("shaders/doodad.vs", "shaders/doodad.fs");
    load(mesh, shader_program, glm::vec3(0.0f, 0.0f, 0.0f), 1.0f);
}

Doodad::Doodad(Mesh* mesh, GLuint shader_program) : Drawable(mesh, shader_program){

}

Doodad::Doodad(Mesh* mesh, GLuint shader_program, glm::vec3 position, GLfloat scale):
    Drawable(mesh, shader_program, position, scale) {
}

void Doodad::updateUniformData(){
    // Set the scale, this is not really going to be a thing, probably
    // ^ It's definitely a thing
    glUniform1f(glGetUniformLocation(shader_program, "scale"), scale);

}
