#ifndef TextureLayer_h
#define TextureLayer_h

#include "texture_loader.h"
#include "texture.hpp"

class TextureLayer {
public:
    TextureLayer();
    TextureLayer(Texture diffuse, Texture splatmap, char channel, int layer_number);

    void setDiffuse(Texture diffuse);
    void setSplatmap(Texture splatmap);
    void setChannel(char channel_char);
    void setChannel(GLuint channel);
    void setLayerNumber(GLuint layer_number);

    Texture getDiffuse();
    Texture getSplatmap();
    GLuint getChannel();
    GLuint getLayerNumber();
    char getChannelChar();

    static int getIntFromChannelChar(char channel);
    static char getCharFromChannelInt(int channel);

private:
    Texture diffuse;
    Texture splatmap;
    GLuint channel;
    GLuint layer_number;
    char channel_char;

};

#endif
