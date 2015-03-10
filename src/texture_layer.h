#ifndef TextureLayer_h
#define TextureLayer_h

#include "texture_loader.h"

class TextureLayer {
public:
    TextureLayer();
    TextureLayer(GLuint diffuse, GLuint splatmap, char channel, GLuint layer_number);

    void setDiffuse(GLuint diffuse);
    void setSplatmap(GLuint splatmap);
    void setChannel(char channel_char);
    void setChannel(GLuint channel);
    void setLayerNumber(GLuint layer_number);

    GLuint getDiffuse();
    GLuint getSplatmap();
    GLuint getChannel();
    GLuint getLayerNumber();

private:
    GLuint diffuse;
    GLuint splatmap;
    GLuint channel;
    GLuint layer_number;
};

#endif
