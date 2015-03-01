#include "shader_loader.h"

std::map<GLuint, std::string> ShaderLoader::shaders;
std::map<std::string, GLuint> ShaderLoader::loaded_shaders;

GLuint ShaderLoader::loadVertexShader(std::string filename){
    // Create the vertex shader
    GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);

    std::string source = GLSLParse(filename);
    const char* vertex_source = source.c_str();

    glShaderSource(vertex_shader, 1, &vertex_source, NULL);

    // Compile it
    glCompileShader(vertex_shader);

    GLint status;
    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &status);
    if (status != GL_TRUE){
        char info_log[512];
        glGetShaderInfoLog(vertex_shader, 512, NULL, info_log);
        printf("Error compiling the vertex shader '%s':\n%s\n", filename.c_str(),
            info_log);
    }

    return vertex_shader;
}

GLuint ShaderLoader::loadFragmentShader(std::string filename){
 // Create the fragment shader
    GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);

    std::string source = GLSLParse(filename);
    const char* fragment_source = source.c_str();

    glShaderSource(fragment_shader, 1, &fragment_source, NULL);

    // Compile it
    glCompileShader(fragment_shader);

    GLint status;
    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &status);
    if (status != GL_TRUE){
        char info_log[512];
        glGetShaderInfoLog(fragment_shader, 512, NULL, info_log);
        printf("Error compiling the fragment shader '%s':\n%s\n", filename.c_str(),
            info_log);
    }

    return fragment_shader;
}

GLuint ShaderLoader::combineShaderProgram(GLuint vertex_shader, GLuint fragment_shader){

    // Combine the shaders into a single program
    GLuint shader_program = glCreateProgram();
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);

    // Put the shader on the graphics card.
    glLinkProgram(shader_program);

    // Tell the fragment shader to use something??
    glBindFragDataLocation(shader_program, 0, "outColor");

    // Free up the space from the vetex and fragment shader because
    // they have been combined into the shader program.
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    return shader_program;
}

GLuint ShaderLoader::loadShaderProgram(std::string vertex, std::string fragment) {
    GLuint shader_program;

    // Check if this combinaton (vs and fs) of shaders has already been loaded
    std::string full_name = vertex + fragment;
    if (loaded_shaders.find(full_name) != loaded_shaders.end()){
        // Just set the shader program to the one that has already
        // been loaded.
        shader_program = loaded_shaders[full_name];
    } else {
        // This is a new combination
        GLuint vertex_shader = ShaderLoader::loadVertexShader(vertex);
        GLuint fragment_shader = ShaderLoader::loadFragmentShader(fragment);
        shader_program = ShaderLoader::combineShaderProgram(vertex_shader, fragment_shader);

        // Update the shader maps
        ShaderLoader::shaders[shader_program] = vertex;
        ShaderLoader::loaded_shaders[full_name] = shader_program;
    }

    return shader_program;
}

std::string ShaderLoader::getShaderName(GLuint shader_id){
    return ShaderLoader::shaders[shader_id];
}

std::string ShaderLoader::GLSLParse(std::string filename){
    // Open the file in read mode
    const char* filename_temp = filename.c_str();
    FILE* file = fopen(filename_temp, "r");

    // // Count the number of characters in the file
    int size_of_file = 0;
    while (fgetc(file) != EOF){
        ++size_of_file;
    }

    // // Create a character array with the correct size
    char* contents = new char[size_of_file + 1];
    // // We need to rewind so that we read from the top again
    rewind(file);

    // // Scan through the file and add each character
    // // to the array.
    int index = 0;
    char current_char;
    while((current_char = fgetc(file)) != EOF){
        contents[index] = current_char;
        index++;
    }

    // // Close the file
    fclose(file);

    // Add the null terminator
    contents[size_of_file] = 0;

    std::string to_return(contents);

    delete [] contents;

    return to_return;
}
