#include "layered_textures.h"

LayeredTextures::LayeredTextures(int size){
    this->num_layers = size;
    this->num_splatmaps = (size - 1) / 3;

    texture_layers = std::vector<TextureLayer>(num_layers);
}

void LayeredTextures::addSplatmap(Texture splatmap){
    if (unique_splatmaps.size() >= num_splatmaps){
        Debug::error("Too many splatmaps for %d textures.\n");
    }
    unique_splatmaps.push_back(splatmap.getGLId());
}

void LayeredTextures::addTexture(Texture diffuse, GLuint splatmap, char channel, int layer_number){
    TextureLayer layer(diffuse, splatmap, channel, layer_number);

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
    TextureLayer layer = getLayer(splatmap, channel);
    return layer.getDiffuse().getGLId();
}

TextureLayer LayeredTextures::getLayer(GLuint splatmap, char channel){
    TextureLayer out_layer;
    for (TextureLayer layer : texture_layers){
        if (layer.getChannelChar() == channel && unique_splatmaps[layer.getSplatmap()] == splatmap){
            out_layer = layer;
        }
    }
    return out_layer;
}

TextureLayer LayeredTextures::getLayer(GLuint layer_number){
    if (layer_number >= 0 && layer_number < num_layers){
        return texture_layers[layer_number];
    } else {
        TextureLayer layer;
        return layer;
    }
}

void LayeredTextures::updateUniforms(Shader shader){
    ////////////////////
    // Diffuse
    glActiveTexture(GL_TEXTURE10);
    glBindTexture(GL_TEXTURE_2D, texture_layers[0].getDiffuse().getGLId());

    glActiveTexture(GL_TEXTURE11);
    glBindTexture(GL_TEXTURE_2D, texture_layers[1].getDiffuse().getGLId());

    glActiveTexture(GL_TEXTURE12);
    glBindTexture(GL_TEXTURE_2D, texture_layers[2].getDiffuse().getGLId());

    glActiveTexture(GL_TEXTURE13);
    glBindTexture(GL_TEXTURE_2D, texture_layers[3].getDiffuse().getGLId());

    glActiveTexture(GL_TEXTURE14);
    glBindTexture(GL_TEXTURE_2D, texture_layers[4].getDiffuse().getGLId());

    glActiveTexture(GL_TEXTURE15);
    glBindTexture(GL_TEXTURE_2D, texture_layers[5].getDiffuse().getGLId());

    glActiveTexture(GL_TEXTURE16);
    glBindTexture(GL_TEXTURE_2D, texture_layers[6].getDiffuse().getGLId());

    ////////////////////
    // Splatmaps
    glActiveTexture(GL_TEXTURE20);
    glBindTexture(GL_TEXTURE_2D, unique_splatmaps[0]);

    glActiveTexture(GL_TEXTURE21);
    glBindTexture(GL_TEXTURE_2D, unique_splatmaps[1]);

    glUniform1i(glGetUniformLocation(shader.getGLId(), "splatmaps[0]"), texture_layers[0].getSplatmap());
    glUniform1i(glGetUniformLocation(shader.getGLId(), "splatmaps[1]"), texture_layers[1].getSplatmap());
    glUniform1i(glGetUniformLocation(shader.getGLId(), "splatmaps[2]"), texture_layers[2].getSplatmap());
    glUniform1i(glGetUniformLocation(shader.getGLId(), "splatmaps[3]"), texture_layers[3].getSplatmap());
    glUniform1i(glGetUniformLocation(shader.getGLId(), "splatmaps[4]"), texture_layers[4].getSplatmap());
    glUniform1i(glGetUniformLocation(shader.getGLId(), "splatmaps[5]"), texture_layers[5].getSplatmap());
    glUniform1i(glGetUniformLocation(shader.getGLId(), "splatmaps[6]"), texture_layers[6].getSplatmap());

    ////////////////////
    // Channels
    glUniform1i(glGetUniformLocation(shader.getGLId(), "channels[0]"),texture_layers[0].getChannel());
    glUniform1i(glGetUniformLocation(shader.getGLId(), "channels[1]"),texture_layers[1].getChannel());
    glUniform1i(glGetUniformLocation(shader.getGLId(), "channels[2]"),texture_layers[2].getChannel());
    glUniform1i(glGetUniformLocation(shader.getGLId(), "channels[3]"),texture_layers[3].getChannel());
    glUniform1i(glGetUniformLocation(shader.getGLId(), "channels[4]"),texture_layers[4].getChannel());
    glUniform1i(glGetUniformLocation(shader.getGLId(), "channels[5]"),texture_layers[5].getChannel());
    glUniform1i(glGetUniformLocation(shader.getGLId(), "channels[6]"),texture_layers[6].getChannel());

}

void LayeredTextures::setTextureLocations(Shader shader){
    ////////////////////
    // Diffuse
    glUniform1i(glGetUniformLocation(shader.getGLId(), "diffuse_textures[0]"), 10);
    glUniform1i(glGetUniformLocation(shader.getGLId(), "diffuse_textures[1]"), 11);
    glUniform1i(glGetUniformLocation(shader.getGLId(), "diffuse_textures[2]"), 12);
    glUniform1i(glGetUniformLocation(shader.getGLId(), "diffuse_textures[3]"), 13);
    glUniform1i(glGetUniformLocation(shader.getGLId(), "diffuse_textures[4]"), 14);
    glUniform1i(glGetUniformLocation(shader.getGLId(), "diffuse_textures[5]"), 15);
    glUniform1i(glGetUniformLocation(shader.getGLId(), "diffuse_textures[6]"), 16);

    ////////////////////
    // Splatmaps
    glUniform1i(glGetUniformLocation(shader.getGLId(), "unique_splatmaps[0]"), 20);
    glUniform1i(glGetUniformLocation(shader.getGLId(), "unique_splatmaps[1]"), 21);
}

void LayeredTextures::swapLayers(GLuint layer1, GLuint layer2){
    if (layer1 == 0 || layer2 == 0){
        Debug::error("Cannot swap with the base layer (layer 0).\n");
        return;
    }
    TextureLayer temp = texture_layers[layer2];
    texture_layers[layer2] = texture_layers[layer1];
    texture_layers[layer1] = temp;

    texture_layers[layer1].setLayerNumber(layer1);
    texture_layers[layer2].setLayerNumber(layer2);
}

string LayeredTextures::asJsonString() {
    string json_string = "";

    // Texture layers
    json_string += "\"texture_layers\": [\n";
    for (TextureLayer& layer : texture_layers) {
        json_string += layer.asJsonString();
    }
    json_string += "]\n";

    return json_string;
}

std::string LayeredTextures::saveData(std::string name){
    // Write the splatmaps out to files
    std::vector<std::string> splatmap_names;
    for (GLuint& splatmap : unique_splatmaps){
        std::string temp_name = name + "_splat_" + std::to_string(splatmap) + ".bmp";
        splatmap_names.push_back(temp_name);
        Texture splat(splatmap);
        splat.save(GL_RGBA, temp_name);
    }

    // Write the diffuse texture layers out to files
    std::vector<std::string> diffuse_names;
    for (TextureLayer& layer : texture_layers){
        GLuint diff_id = layer.getDiffuse().getGLId();
        std::string temp_name = name + "_diff_" + std::to_string(diff_id) + ".bmp";
        diffuse_names.push_back(temp_name);
        Texture diff(diff_id);
        diff.save(GL_RGBA, temp_name);
    }

    // Generate a string based on the map file spec
    std::string map_output = "";

    for (const std::string& splatmap_name : splatmap_names){
        map_output += "s " + splatmap_name + "\n";
    }

    int i = 0;
    for (TextureLayer& layer : texture_layers){
        map_output += "g " + std::to_string(layer.getLayerNumber()) + " " +
            std::to_string(layer.getSplatmap()) + " " +
            TextureLayer::getCharFromChannelInt(layer.getChannel()) + " " +
            diffuse_names[i] + "\n";
        i++;
    }

    return map_output;
}

bool LayeredTextures::needsSplatmaps(){
    return num_splatmaps > unique_splatmaps.size();
}

int LayeredTextures::getNumLayers(){
    return num_layers;
}
