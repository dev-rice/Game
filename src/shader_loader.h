#ifndef ShaderLoader_h
#define ShaderLoader_h

#if defined __APPLE__ && __MACH__
    #include <OpenGL/OpenGL.h>
// #elif defined __gnu_linux__
#endif

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <stdio.h>

class ShaderLoader {
public:
    static GLuint loadVertexShader(const char*);
    static GLuint loadFragmentShader(const char*);
    static GLuint combineShaderProgram(GLuint, GLuint);

private:
    static const char* GLSLParse(const char*);

};

#endif