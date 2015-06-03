#include "texture_layer.h"

TextureLayer::TextureLayer(){
    diffuse = Texture(glm::vec4(0.0, 0.0, 0.0, 1.0));
    splatmap = 0;
    channel = 1;
    setLayerNumber(0);
}

TextureLayer::TextureLayer(Texture diffuse, GLuint splatmap, char channel, int layer_number){
    setDiffuse(diffuse);
    setSplatmap(splatmap);
    setChannel(channel);
    setLayerNumber(layer_number);
}

void TextureLayer::setDiffuse(Texture diffuse){
    this->diffuse = diffuse;
}

void TextureLayer::setSplatmap(GLuint splatmap){
    this->splatmap = splatmap;
}

void TextureLayer::setChannel(char channel_char){
    this->channel_char = channel_char;
    setChannel((GLuint)getIntFromChannelChar(channel_char));
}

void TextureLayer::setChannel(GLuint channel){
    this->channel = channel;
}

void TextureLayer::setLayerNumber(GLuint layer_number){
    this->layer_number = layer_number;
}

Texture TextureLayer::getDiffuse(){
    return diffuse;
}

GLuint TextureLayer::getSplatmap(){
    return splatmap;
}

GLuint TextureLayer::getChannel(){
    return channel;
}

GLuint TextureLayer::getLayerNumber(){
    return layer_number;
}

char TextureLayer::getChannelChar(){
    return channel_char;
}

int TextureLayer::getIntFromChannelChar(char channel){
    int channel_int = 0;

    if (channel == 'r'){
        channel_int = 1;
    } else if (channel == 'g'){
        channel_int = 2;
    } else if (channel == 'b'){
        channel_int = 3;
    }

    return channel_int;
}

char TextureLayer::getCharFromChannelInt(int channel){
    char channel_char = 'r';

    if (channel == 1){
        channel_char = 'r';
    } else if (channel == 2){
        channel_char = 'g';
    } else if (channel == 3){
        channel_char = 'b';
    }

    return channel_char;
}
