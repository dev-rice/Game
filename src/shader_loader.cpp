#include "shader_loader.h"

GLuint ShaderLoader::loadVertexShader(const char* filename){
    // Create the vertex shader
    GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    const char* vertex_source = GLSLParse(filename);
    glShaderSource(vertex_shader, 1, &vertex_source, NULL);

    // Compile it
    glCompileShader(vertex_shader);

    GLint status;
    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &status);
    if (status != GL_TRUE){
        char info_log[512];
        glGetShaderInfoLog(vertex_shader, 512, NULL, info_log);
        printf("Error compiling the vertex shader:\n%s\n", info_log);
    }

    return vertex_shader;
} 

GLuint ShaderLoader::loadFragmentShader(const char* filename){
 // Create the fragment shader
    GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    const char* fragment_source = GLSLParse(filename);
    glShaderSource(fragment_shader, 1, &fragment_source, NULL);
    
    // Compile it
    glCompileShader(fragment_shader);

    GLint status;
    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &status);
    if (status != GL_TRUE){
        char info_log[512];
        glGetShaderInfoLog(fragment_shader, 512, NULL, info_log);
        printf("Error compiling the fragment shader:\n%s\n", info_log);
    }

    return fragment_shader;
}

GLuint ShaderLoader::combineShaderProgram(GLuint vertex_shader, GLuint fragment_shader){

    // Combine the shaders into a single program
    GLuint shader_program = glCreateProgram();
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);

    // Tell the fragment shader to use something??
    // glBindFragDataLocation(shader_program, 0, "outColor");

    // Put the shader on the graphics card.
    glLinkProgram(shader_program);

    // Free up the space from the vetex and fragment shader because
    // they have been combined into the shader program.
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    return shader_program;
}

const char* ShaderLoader::GLSLParse(const char* filename){
    // Open the file in read mode
    FILE* file = fopen(filename, "r");

    // Count the number of characters in the file
    int size_of_file = 0;
    while (fgetc(file) != EOF){
        ++size_of_file;
    }

    // Create a character array with the correct size
    char* contents = new char[size_of_file];
    // We need to rewind so that we read from the top again
    rewind(file);

    // Scan through the file and add each character 
    // to the array. 
    int index = 0;
    char current_char;
    while((current_char = fgetc(file)) != EOF){
        contents[index] = current_char;
        index++;
    }    

    // Close the file
    fclose(file);

    // Add the null terminator
    contents[size_of_file] = 0;
    
    return contents;
}