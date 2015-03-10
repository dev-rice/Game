#include "texture_layer.h"

TextureLayer::TextureLayer(){
    diffuse = TextureLoader::loadTextureFromPixel(0.0, 0.0, 0.0, 1.0);
    splatmap = TextureLoader::loadTextureFromPixel(0.0, 0.0, 0.0, 1.0);
    channel = 1;
    layer_number = 0;
}

TextureLayer::TextureLayer(GLuint diffuse, GLuint splatmap, char channel, GLuint layer_number){
    setDiffuse(diffuse);
    setSplatmap(splatmap);
    setChannel(channel);
    setLayerNumber(layer_number);
}

void TextureLayer::setDiffuse(GLuint diffuse){
    this->diffuse = diffuse;
}

void TextureLayer::setSplatmap(GLuint splatmap){
    this->splatmap = splatmap;
}

void TextureLayer::setChannel(char channel_char){
    if (channel_char == 'r'){
        setChannel((GLuint)1);
    } else if (channel_char == 'g'){
        setChannel((GLuint)2);
    } else if (channel_char == 'b'){
        setChannel((GLuint)3);
    } else {
        setChannel((GLuint)0);
    }
}

void TextureLayer::setChannel(GLuint channel){
    this->channel = channel;
}

void TextureLayer::setLayerNumber(GLuint layer_number){
    this->layer_number = layer_number;
}

GLuint TextureLayer::getDiffuse(){
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
