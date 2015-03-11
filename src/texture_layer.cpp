#include "texture_layer.h"

TextureLayer::TextureLayer(){
    diffuse = TextureLoader::loadTextureFromPixel(0.0, 0.0, 0.0, 1.0);
    splatmap = TextureLoader::loadTextureFromPixel(0.0, 0.0, 0.0, 1.0);
    channel = 1;
}

TextureLayer::TextureLayer(GLuint diffuse, GLuint splatmap, char channel){
    setDiffuse(diffuse);
    setSplatmap(splatmap);
    setChannel(channel);
}

void TextureLayer::setDiffuse(GLuint diffuse){
    this->diffuse = diffuse;
}

void TextureLayer::setSplatmap(GLuint splatmap){
    this->splatmap = splatmap;
}

void TextureLayer::setChannel(char channel_char){
    this->channel_char = channel_char;
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

GLuint TextureLayer::getDiffuse(){
    return diffuse;
}

GLuint TextureLayer::getSplatmap(){
    return splatmap;
}

GLuint TextureLayer::getChannel(){
    return channel;
}

char TextureLayer::getChannelChar(){
    return channel_char;
}
