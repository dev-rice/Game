#ifndef Shader_h
#define Shader_h

#include <string>
#include <fstream>

#include "includes/gl.hpp"
#include "debug.h"

using namespace std;

class Shader {
public:
    Shader(GLuint);
    Shader(string vs_filename, string fs_filename);

    GLuint getGLId();
private:

    GLuint loadVertexShader(string vs_filename);
    GLuint loadFragmentShader(string fs_filename);
    GLuint combineShaderProgram(GLuint vertex_shader, GLuint fragment_shader);
    GLuint loadShaderProgram(string vs_filename, string fs_filename);
    string GLSLParse(string);

    GLuint gl_shader_id;

};

#endif
