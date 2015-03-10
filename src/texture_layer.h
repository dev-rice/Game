#ifndef TextureLayer_h
#define TextureLayer_h

#include "texture_loader.h"

class TextureLayer {
public:
    TextureLayer();
    TextureLayer(GLuint diffuse, GLuint splatmap, char channel);

    void setDiffuse(GLuint diffuse);
    void setSplatmap(GLuint splatmap);
    void setChannel(char channel_char);
    void setChannel(GLuint channel);

    GLuint getDiffuse();
    GLuint getSplatmap();
    GLuint getChannel();

private:
    GLuint diffuse;
    GLuint splatmap;
    GLuint channel;
};

#endif
