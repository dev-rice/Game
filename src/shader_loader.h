#ifndef ShaderLoader_h
#define ShaderLoader_h

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#if defined __APPLE__ && __MACH__
    #include <OpenGL/OpenGL.h>
// #elif defined __gnu_linux__
#endif

#include <stdio.h>
#include <string>

class ShaderLoader {
public:
    static GLuint loadVertexShader(const char*);
    static GLuint loadFragmentShader(const char*);
    static GLuint combineShaderProgram(GLuint, GLuint);

private:
    static std::string GLSLParse(const char*);

};

#endif