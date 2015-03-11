#ifndef TextureLayer_h
#define TextureLayer_h

#include "texture_loader.h"

class TextureLayer {
public:
    TextureLayer();
    TextureLayer(GLuint diffuse, GLuint splatmap, char channel, int layer_number);

    void setDiffuse(GLuint diffuse);
    void setSplatmap(GLuint splatmap);
    void setChannel(char channel_char);
    void setChannel(GLuint channel);
    void setLayerNumber(GLuint layer_number);

    GLuint getDiffuse();
    GLuint getSplatmap();
    GLuint getChannel();
    GLuint getLayerNumber();
    char getChannelChar();

    static int getIntFromChannelChar(char channel);
    static char getCharFromChannelInt(int channel);

private:
    GLuint diffuse;
    GLuint splatmap;
    GLuint channel;
    GLuint layer_number;
    char channel_char;

};

#endif
