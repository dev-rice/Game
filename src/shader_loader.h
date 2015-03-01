#ifndef ShaderLoader_h
#define ShaderLoader_h

#include <GL/glew.h>

#if defined __APPLE__ && __MACH__
    #include <OpenGL/OpenGL.h>
// #elif defined __gnu_linux__
#endif

#include <stdio.h>
#include <string>
#include <map>

#include "debug.h"

class ShaderLoader {
public:
    static GLuint loadVertexShader(std::string);
    static GLuint loadFragmentShader(std::string);
    static GLuint combineShaderProgram(GLuint, GLuint);
    static GLuint loadShaderProgram(std::string, std::string);
    static std::string getShaderName(GLuint);

private:
    static std::string GLSLParse(std::string);

    static std::map<GLuint, std::string> shaders;
    static std::map<std::string, GLuint> loaded_shaders;

};

#endif
