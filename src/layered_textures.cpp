#include "layered_textures.h"

LayeredTextures::LayeredTextures(int size){
    this->num_layers = size;
    this->num_splatmaps = (size - 1) / 3;

    texture_layers = std::vector<TextureLayer>(num_layers);
}

void LayeredTextures::addSplatmap(GLuint splatmap){
    if (unique_splatmaps.size() >= num_splatmaps){
        Debug::error("Too many splatmaps for %d textures.\n");
    }
    unique_splatmaps.push_back(splatmap);
}

void LayeredTextures::addTexture(GLuint diffuse, GLuint splatmap, char channel, int layer_number){
    TextureLayer layer(diffuse, splatmap, channel);

    if (layer_number >= num_layers){
        Debug::error("Layer number out of bounds %d. Range is [0, %d]\n", layer_number, num_layers - 1);
    } else if (splatmap >= num_splatmaps){
        Debug::error("Splatmap number out of bounds %d. Range is [0, %d]\n", splatmap, num_splatmaps - 1);
    } else {
        texture_layers[layer_number] = layer;
    }
}

GLuint LayeredTextures::getSplatmap(int index){
    if (index >= 0 && index < num_splatmaps){
        return unique_splatmaps[index];
    } else {
        return 0;
    }
}

GLuint LayeredTextures::getTexture(GLuint splatmap, char channel){
    GLuint texture = 0;

    // Linear search because lazy and this is a relatively
    // smale search space
    for (TextureLayer layer : texture_layers){
        if (layer.getChannelChar() == channel && unique_splatmaps[layer.getSplatmap()] == splatmap){
            texture = layer.getDiffuse();
        }
    }

    return texture;
}

void LayeredTextures::updateUniforms(GLuint shader_program){
    ////////////////////
    // Diffuse
    glActiveTexture(GL_TEXTURE10);
    glBindTexture(GL_TEXTURE_2D, texture_layers[0].getDiffuse());

    glActiveTexture(GL_TEXTURE11);
    glBindTexture(GL_TEXTURE_2D, texture_layers[1].getDiffuse());

    glActiveTexture(GL_TEXTURE12);
    glBindTexture(GL_TEXTURE_2D, texture_layers[2].getDiffuse());

    glActiveTexture(GL_TEXTURE13);
    glBindTexture(GL_TEXTURE_2D, texture_layers[3].getDiffuse());

    glActiveTexture(GL_TEXTURE14);
    glBindTexture(GL_TEXTURE_2D, texture_layers[4].getDiffuse());

    glActiveTexture(GL_TEXTURE15);
    glBindTexture(GL_TEXTURE_2D, texture_layers[5].getDiffuse());

    glActiveTexture(GL_TEXTURE16);
    glBindTexture(GL_TEXTURE_2D, texture_layers[6].getDiffuse());

    ////////////////////
    // Splatmaps
    glActiveTexture(GL_TEXTURE20);
    glBindTexture(GL_TEXTURE_2D, unique_splatmaps[0]);

    glActiveTexture(GL_TEXTURE21);
    glBindTexture(GL_TEXTURE_2D, unique_splatmaps[1]);

    glUniform1i(glGetUniformLocation(shader_program, "splatmaps[0]"), texture_layers[0].getSplatmap());
    glUniform1i(glGetUniformLocation(shader_program, "splatmaps[1]"), texture_layers[1].getSplatmap());
    glUniform1i(glGetUniformLocation(shader_program, "splatmaps[2]"), texture_layers[2].getSplatmap());
    glUniform1i(glGetUniformLocation(shader_program, "splatmaps[3]"), texture_layers[3].getSplatmap());
    glUniform1i(glGetUniformLocation(shader_program, "splatmaps[4]"), texture_layers[4].getSplatmap());
    glUniform1i(glGetUniformLocation(shader_program, "splatmaps[5]"), texture_layers[5].getSplatmap());
    glUniform1i(glGetUniformLocation(shader_program, "splatmaps[6]"), texture_layers[6].getSplatmap());

    ////////////////////
    // Channels
    glUniform1i(glGetUniformLocation(shader_program, "channels[0]"),texture_layers[0].getChannel());
    glUniform1i(glGetUniformLocation(shader_program, "channels[1]"),texture_layers[1].getChannel());
    glUniform1i(glGetUniformLocation(shader_program, "channels[2]"),texture_layers[2].getChannel());
    glUniform1i(glGetUniformLocation(shader_program, "channels[3]"),texture_layers[3].getChannel());
    glUniform1i(glGetUniformLocation(shader_program, "channels[4]"),texture_layers[4].getChannel());
    glUniform1i(glGetUniformLocation(shader_program, "channels[5]"),texture_layers[5].getChannel());
    glUniform1i(glGetUniformLocation(shader_program, "channels[6]"),texture_layers[6].getChannel());

}

void LayeredTextures::setTextureLocations(GLuint shader_program){
    ////////////////////
    // Diffuse
    glUniform1i(glGetUniformLocation(shader_program, "diffuse_textures[0]"), 10);
    glUniform1i(glGetUniformLocation(shader_program, "diffuse_textures[1]"), 11);
    glUniform1i(glGetUniformLocation(shader_program, "diffuse_textures[2]"), 12);
    glUniform1i(glGetUniformLocation(shader_program, "diffuse_textures[3]"), 13);
    glUniform1i(glGetUniformLocation(shader_program, "diffuse_textures[4]"), 14);
    glUniform1i(glGetUniformLocation(shader_program, "diffuse_textures[5]"), 15);
    glUniform1i(glGetUniformLocation(shader_program, "diffuse_textures[6]"), 16);

    ////////////////////
    // Splatmaps
    glUniform1i(glGetUniformLocation(shader_program, "unique_splatmaps[0]"), 20);
    glUniform1i(glGetUniformLocation(shader_program, "unique_splatmaps[1]"), 21);
}
